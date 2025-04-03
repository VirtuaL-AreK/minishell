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

char *process_ansi_c(const char *s)
{
    int i = 0, j = 0;
    int len = ft_strlen(s);
    char *result = malloc(len + 1);
    if (!result)
        return NULL;
    while (s[i])
    {
        if (s[i] == '\\')
        {
            i++;
            if (s[i] == 'n')
                result[j++] = '\n';
            else if (s[i] == 't')
                result[j++] = '\t';
            else if (s[i] == '\\')
                result[j++] = '\\';
            else if (s[i] == '\'')
                result[j++] = '\'';
            else
                result[j++] = s[i];
            i++;
        }
        else
        {
            result[j++] = s[i++];
        }
    }
    result[j] = '\0';
    return result;
}

char *process_dollar_dquote(const char *s)
{
    int i = 0, j = 0;
    int len = ft_strlen(s);
    char *result = malloc(len + 1);
    if (!result)
        return NULL;
    while (s[i])
    {
        if (s[i] == '\\')
        {
            i++;
            if (s[i] == 'n')
                result[j++] = '\n';
            else if (s[i] == 't')
                result[j++] = '\t';
            else if (s[i] == '\\')
                result[j++] = '\\';
            else if (s[i] == '"')
                result[j++] = '"';
            else
                result[j++] = s[i];
            i++;
        }
        else
        {
            result[j++] = s[i++];
        }
    }
    result[j] = '\0';
    return result;
}


char *expand_string(const char *str, t_shell *shell)
{
    char buffer[4096];
    int idx = 0;
    int i = 0;
    int in_sq = 0;
    int in_dq = 0;

    while (str[i] && idx < 4095)
    {
        // traitement spécial pour $'…' ou $"…" si l'on n'est pas dans une quote simple
        if (!in_sq && str[i] == '$' && (str[i+1] == '\'' || str[i+1] == '"'))
        {
            char quote = str[i+1];  // quote ouvrant
            i += 2; // saute le '$' et le quote ouvrant
            int seg_start = i;
            while (str[i] && str[i] != quote)
                i++;
            int seg_len = i - seg_start;
            char *segment = strndup(str + seg_start, seg_len);
            char *processed = NULL;
            if (quote == '\'')
                processed = process_ansi_c(segment);
            else
                processed = process_dollar_dquote(segment);
            free(segment);
            {
                int k = 0;
                while (processed[k] && idx < 4095)
                    buffer[idx++] = processed[k++];
            }
            free(processed);
            if (str[i] == quote)
                i++;  // passe le quote fermant
            continue;
        }
        //traitement des variables $? ou $VAR hors quotes simples
        else if (!in_sq && str[i] == '$')
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
                continue;
            }
            else if ((str[i] >= 'A' && str[i] <= 'Z') ||
                     (str[i] >= 'a' && str[i] <= 'z') ||
                     (str[i] == '_'))
            {
                int start = i;
                while (str[i] && ((str[i] >= 'A' && str[i] <= 'Z') ||
                                  (str[i] >= 'a' && str[i] <= 'z') ||
                                  (str[i] >= '0' && str[i] <= '9') ||
                                  (str[i] == '_')))
                    i++;
                int var_len = i - start;
                char *var_name = strndup(str + start, var_len);
                char *val = get_local_env_value(var_name, shell);
                free(var_name);
                int k = 0;
                while (val[k] && idx < 4095)
                    buffer[idx++] = val[k++];
                free(val);
                continue;
            }
            else
            {
                buffer[idx++] = '$';
                continue;
            }
        }
        //gestion des quotes groupantes classiques
        else if (str[i] == '\'' && !in_dq)
        {
            in_sq = !in_sq;
            i++;
            continue;
        }
        else if (str[i] == '"' && !in_sq)
        {
            in_dq = !in_dq;
            i++;
            continue;
        }
        else if (str[i] == '\\')
        {
            i++;
            if (str[i])
            {
                buffer[idx++] = str[i++];
            }
            continue;
        }
        else
        {
            buffer[idx++] = str[i++];
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


