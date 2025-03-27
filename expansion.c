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
    char buffer[4096];
    int  idx = 0; 
    int  i   = 0; 

    int  in_sq = 0; 
    int  in_dq = 0;

    while (str[i])
    {
        if (str[i] == '\'' && !in_dq)
        {
            in_sq = !in_sq; 
            i++;
            continue;
        }

        if (str[i] == '"' && !in_sq)
        {
            in_dq = !in_dq;
            i++;
            continue;
        }

        if (str[i] == '\\')
        {
            int count = 0;
            while (str[i] == '\\')
            {
                count++;
                i++;
            }
            if (!in_sq && str[i] == '$')
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

        if (str[i] == '$' && !in_sq)
        {
            i++;
            if (str[i] == '?')
            {
                i++;
                char *exit_str = ft_itoa(shell->exit_status);
                for (int k = 0; exit_str[k] && idx < 4095; k++)
                    buffer[idx++] = exit_str[k];
                free(exit_str);
                continue;
            }

            int start = i;
            while (str[i] && (isalnum((unsigned char)str[i]) || str[i] == '_'))
                i++;
            int var_len = i - start;
            if (var_len > 0)
            {
                char *var_name = strndup(str + start, var_len);
                char *val = get_local_env_value(var_name, shell); // récupère la valeur
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
                buffer[idx++] = str[i];
            i++;
        }
    }

    buffer[idx] = '\0';
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


