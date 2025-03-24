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

    while (str[i])
    {
        if (str[i] == '\\')
        {
            int count = 0;
            // Compter les backslashes consécutifs
            while (str[i] == '\\')
            {
                count++;
                i++;
            }
            // Si le caractère suivant est un '$'
            if (str[i] == '$')
            {
                if (count % 2 == 1)
                {
                    // Nombre impair : le dernier backslash échappe le '$'
                    int bs_to_print = count / 2; // paires complètes
                    for (int k = 0; k < bs_to_print && idx < 4095; k++)
                        buffer[idx++] = '\\';
                    if (idx < 4095)
                        buffer[idx++] = '$';
                    i++; // On saute le '$' échappé sans expansion
                }
                else
                {
                    // Nombre pair : on affiche count/2 backslashes et on traite '$'
                    int bs_to_print = count / 2;
                    for (int k = 0; k < bs_to_print && idx < 4095; k++)
                        buffer[idx++] = '\\';
                    i++; // Passer le '$'
                    // Expansion de la variable
                    if (str[i] == '?')
                    {
                        i++;
                        char *exit_str = ft_itoa(shell->exit_status);
                        for (int k = 0; exit_str[k] && idx < 4095; k++)
                            buffer[idx++] = exit_str[k];
                        free(exit_str);
                    }
                    else if (isalnum((unsigned char)str[i]) || str[i] == '_')
                    {
                        int start = i;
                        while (str[i] && (isalnum((unsigned char)str[i]) || str[i] == '_'))
                            i++;
                        char *var_name = strndup(str + start, i - start);
                        char *val = get_local_env_value(var_name, shell);
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
            }
            else
            {
                // Pas de '$' après : on recopie tous les backslashes littéralement
                for (int k = 0; k < count && idx < 4095; k++)
                    buffer[idx++] = '\\';
            }
        }
        else if (str[i] == '$')
        {
            i++;
            if (str[i] == '?')
            {
                i++;
                char *exit_str = ft_itoa(shell->exit_status);
                for (int k = 0; exit_str[k] && idx < 4095; k++)
                    buffer[idx++] = exit_str[k];
                free(exit_str);
            }
            else if (isalnum((unsigned char)str[i]) || str[i] == '_')
            {
                int start = i;
                while (str[i] && (isalnum((unsigned char)str[i]) || str[i] == '_'))
                    i++;
                char *var_name = strndup(str + start, i - start);
                char *val = get_local_env_value(var_name, shell);
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
        // On n'expanse que si le token doit l'être
        if (cur->type == TOKEN_WORD && cur->should_expand)
        {
            char *expanded = expand_string(cur->value, shell);
            free(cur->value);
            cur->value = expanded;
        }
        cur = cur->next;
    }
}

