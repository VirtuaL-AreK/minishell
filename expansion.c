#include "minishell.h"

static char *get_local_env_value(const char *var, t_shell *shell)
{
    int i = 0;
    int len = ft_strlen(var);
    while (shell->env && shell->env[i])
    {
        if (ft_strncmp(shell->env[i], var, len) == 0 && shell->env[i][len] == '=')
            return ft_strdup(shell->env[i] + len + 1);
        i++;
    }
    return ft_strdup("");
}

char *expand_string(const char *str, t_shell *shell)
{
    char *input_str;

    /* Si le token commence par '~' et que le caractère suivant est '\0' ou '/' */
    if (str[0] == '~' && (str[1] == '\0' || str[1] == '/'))
    {
        char *home = get_local_env_value("HOME", shell);
        if (!home)
            home = ft_strdup("");  // Si HOME n'est pas défini
        input_str = ft_strjoin(home, str + 1);  // Concatène HOME et le reste
        free(home);
    }
    else
    {
        input_str = ft_strdup(str);
    }

    char buffer[4096];
    int idx = 0;
    int i = 0;
    int in_sq = 0;
    int in_dq = 0;

    while (input_str[i])
    {
        if (input_str[i] == '\'' && !in_dq)
        {
            in_sq = !in_sq;
            i++;
            continue;
        }

        if (input_str[i] == '"' && !in_sq)
        {
            in_dq = !in_dq;
            i++;
            continue;
        }

        if (input_str[i] == '\\')
        {
            int count = 0;
            while (input_str[i] == '\\')
            {
                count++;
                i++;
            }
            if (!in_sq && input_str[i] == '$')
            {
                if (count % 2 == 1)
                {
                    int bs_to_print = count / 2; // paires complètes
                    for (int k = 0; k < bs_to_print && idx < 4095; k++)
                        buffer[idx++] = '\\';
                    if (idx < 4095)
                        buffer[idx++] = '$';
                    i++;
                }
                else
                {
                    int bs_to_print = count / 2;
                    for (int k = 0; k < bs_to_print && idx < 4095; k++)
                        buffer[idx++] = '\\';
                    i--;
                }
            }
            else
            {
                for (int k = 0; k < count && idx < 4095; k++)
                    buffer[idx++] = '\\';
            }
            continue;
        }

        if (input_str[i] == '$' && !in_sq)
        {
            i++;
            if (input_str[i] == '?')
            {
                i++;
                char *exit_str = ft_itoa(shell->exit_status);
                for (int k = 0; exit_str[k] && idx < 4095; k++)
                    buffer[idx++] = exit_str[k];
                free(exit_str);
                continue;
            }

            int start = i;
            while (input_str[i] && (isalnum((unsigned char)input_str[i]) || input_str[i] == '_'))
                i++;
            int var_len = i - start;
            if (var_len > 0)
            {
                char *var_name = strndup(input_str + start, var_len);
                char *val = get_local_env_value(var_name, shell); // Récupère la valeur
                free(var_name);
                for (int k = 0; val[k] && idx < 4095; k++)
                    buffer[idx++] = val[k];
                free(val);
            }
            else
            {
                if (idx < 4095)
                    buffer[idx++] = '$';
            }
        }
        else
        {
            if (idx < 4095)
                buffer[idx++] = input_str[i];
            i++;
        }
    }

    buffer[idx] = '\0';
    free(input_str);
    return ft_strdup(buffer);
}




void expand_tokens(t_token *tokens, t_shell *shell)
{
    t_token *cur = tokens;
    while (cur)
    {
        if (cur->type == TOKEN_WORD && cur->should_expand)
        {
            char *expanded = expand_string(cur->value, shell);
            free(cur->value);
            cur->value = expanded;
        }
        cur = cur->next;
    }
}


