#include "minishell.h"

static char *get_env_value(const char *var, char **env)
{
    int   i = 0;
    size_t len = strlen(var);

    while (env && env[i])
    {
        if (strncmp(env[i], var, len) == 0 && env[i][len] == '=')
        {
            return strdup(env[i] + len + 1);
        }
        i++;
    }
    return strdup("");
}

static char *expand_string(const char *str, char **env)
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
                char *exit_str = ft_itoa(gexitstatus);
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

                char *val = get_env_value(var_name, env);
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
    return strdup(buffer);
}

void expand_tokens(t_token *tokens, char **env)
{
    t_token *curr = tokens;
    while (curr)
    {
        if (curr->type == TOKEN_WORD)
        {
            if (curr->has_single_quote == 0)
            {
                char *expanded = expand_string(curr->value, env);
                free(curr->value);
                curr->value = expanded;
            }
        }
        curr = curr->next;
    }
}
