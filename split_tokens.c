#include "minishell.h"

int ft_isspace(int c)
{
    return (c == ' ' || c == '\t' || c == '\n' ||
            c == '\r' || c == '\v' || c == '\f');
}

void skip_spaces(const char *input, int *i)
{
    while (input[*i] && ft_isspace(input[*i]))
        (*i)++;
}

int is_special(char c)
{
    return (c == '|' || c == '&' || c == '<' || c == '>');
}

char *remove_quotes(const char *s)
{
    int i = 0, j = 0;
    int len = ft_strlen(s);
    char *res = malloc(len + 1);
    if (!res)
        return NULL;
    while (s[i])
    {
        if (s[i] != '"' && s[i] != '\'')
            res[j++] = s[i];
        i++;
    }
    res[j] = '\0';
    return res;
}

int count_tokens(const char *input)
{
    int count = 0, i = 0;

    skip_spaces(input, &i);
    while (input[i])
    {
        if (is_special(input[i]))
        {
            count++;
            if (input[i] == '>' && input[i + 1] == '>')
                i++;
            i++;
        }
        else
        {
            count++;
            while (input[i] && !ft_isspace(input[i]) && !is_special(input[i]))
            {
                if (input[i] == '"' || input[i] == '\'')
                {
                    char quote = input[i++];
                    while (input[i] && input[i] != quote)
                        i++;
                    if (input[i] == quote)
                        i++;
                }
                else
                    i++;
            }
        }
        skip_spaces(input, &i);
    }
    return count;
}

char **ft_split_command(const char *input)
{
    int token_count = count_tokens(input);
    char **tokens = malloc((token_count + 1) * sizeof(char *));
    int i = 0, j = 0, start;
    char quote;

    if (!tokens)
        return NULL;

    skip_spaces(input, &i);
    while (input[i])
    {
        if (is_special(input[i]))
        {
            start = i;
            if ((input[i] == '|' && input[i + 1] == '|') ||
                (input[i] == '&' && input[i + 1] == '&') ||
                (input[i] == '>' && input[i + 1] == '>'))
                i++;
            i++;
            tokens[j++] = ft_substr(input, start, i - start);
        }
        else
        {
            start = i;
            while (input[i] && !ft_isspace(input[i]) && !is_special(input[i]))
            {
                if (input[i] == '"' || input[i] == '\'')
                {
                    quote = input[i++];
                    while (input[i] && input[i] != quote)
                        i++;
                    if (input[i] == quote)
                        i++;
                }
                else
                {
                    i++;
                }
            }
            char *token = ft_substr(input, start, i - start);
            char *cleaned = remove_quotes(token);
            free(token);
            tokens[j++] = cleaned;
        }
        skip_spaces(input, &i);
    }
    tokens[j] = NULL;
    return tokens;
}
