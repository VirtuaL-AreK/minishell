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

    /* Gestion de l'expansion de la tilde */
    if (str[0] == '~' && (str[1] == '\0' || str[1] == '/'))
    {
        char *home = get_local_env_value("HOME", shell);
        if (!home)
            home = ft_strdup("");
        input_str = ft_strjoin(home, str + 1);
        free(home);
    }
    else
    {
        input_str = ft_strdup(str);
    }

    char buffer[4096];
    int idx = 0;
    int i = 0;
    int in_sq = 0; // flag : dans une quote simple
    int in_dq = 0; // flag : dans une quote double

    while (input_str[i] && idx < 4095)
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
            /* Compter le nombre de backslashes consécutifs */
            while (input_str[i] && input_str[i] == '\\')
            {
                count++;
                i++;
            }
            /* Si le caractère suivant est '$' et que l'on n'est pas dans une quote simple */
            if (!in_sq && input_str[i] == '$')
            {
                if (count % 2 == 1)
                {
                    /* Nombre impair de '\' : le '$' est échappé
                       On imprime count/2 backslashes littéraux et un '$' */
                    int k = 0;
                    while (k < count / 2 && idx < 4095)
                    {
                        buffer[idx++] = '\\';
                        k++;
                    }
                    if (idx < 4095)
                        buffer[idx++] = '$';
                    i++; /* Consommer le '$' échappé */
                }
                else
                {
                    /* Nombre pair de '\' : on imprime count/2 backslashes,
                       et on laisse le '$' non consommé pour expansion */
                    int k = 0;
                    while (k < count / 2 && idx < 4095)
                    {
                        buffer[idx++] = '\\';
                        k++;
                    }
                    /* Ne consommer PAS le '$' ici, il sera traité dans l'itération suivante */
                }
            }
            else
            {
                /* Pas suivi d'un '$' ou dans une quote simple :
                   On imprime tous les '\' */
                int k = 0;
                while (k < count && idx < 4095)
                {
                    buffer[idx++] = '\\';
                    k++;
                }
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
                int k = 0;
                while (exit_str[k] && idx < 4095)
                {
                    buffer[idx++] = exit_str[k];
                    k++;
                }
                free(exit_str);
                continue;
            }
            int start = i;
            while (input_str[i] && (isalnum((unsigned char)input_str[i]) || input_str[i] == '_'))
            {
                i++;
            }
            int var_len = i - start;
            if (var_len > 0)
            {
                char *var_name = strndup(input_str + start, var_len);
                char *val = get_local_env_value(var_name, shell); // Récupère la valeur de la variable
                free(var_name);
                int k = 0;
                while (val[k] && idx < 4095)
                {
                    buffer[idx++] = val[k];
                    k++;
                }
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


