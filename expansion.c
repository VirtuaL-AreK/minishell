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

static char *expand_string(const char *str, t_shell *shell)
{
    char buffer[4096];
    int idx = 0;
    int i = 0;

    buffer[0] = '\0';

    while (str[i])
    {
        if (str[i] == '$')
        {
            i++;
            if (str[i] == '?')
            {
                i++;
                char *exit_str = ft_itoa(shell->exit_status);
                int k = 0;
                while (exit_str[k] && idx < 4095)
                    buffer[idx++] = exit_str[k++];
                free(exit_str);
            }
            else if (isalnum((unsigned char)str[i]) || str[i] == '_')
            {
                int start = i;
                while (str[i] && (isalnum((unsigned char)str[i]) || str[i] == '_'))
                    i++;
                int length = i - start;
                char *var_name = strndup(str + start, length);

                char *val = get_local_env_value(var_name, shell);
                free(var_name);

                // on recopie val dans buffer
                int k = 0;
                while (val[k] && idx < 4095)
                    buffer[idx++] = val[k++];
                free(val);
            }
            else
            {
                // on recopie le '$' tel quel
                if (idx < 4095)
                    buffer[idx++] = '$';
            }
        }
        else
        {
            // caractère normal => on recopie
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
        if (cur->type == TOKEN_WORD && !cur->has_single_quote)
        {
            char *expanded = expand_string(cur->value, shell);
            free(cur->value);
            cur->value = expanded;
        }
        cur = cur->next;
    }
}
