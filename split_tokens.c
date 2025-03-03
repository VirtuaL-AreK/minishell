#include "minishell.h"

int ft_isspace(int c)
{
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f')
        return 1;
    return 0;
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

int count_tokens(const char *input)
{
    int count = 0, i = 0;
    char quote;

    skip_spaces(input, &i);
    while (input[i])
    {
        count++;
        if (is_special(input[i]))
        {
            if ((input[i] == '|' && input[i + 1] == '|') ||
                (input[i] == '&' && input[i + 1] == '&') ||
                (input[i] == '>' && input[i + 1] == '>'))
                i++;
            i++;
        }
        else if (input[i] == '"' || input[i] == '\'')
        {
            quote = input[i++];
            while (input[i] && input[i] != quote)
                i++;
            if (input[i] == quote)
                i++;
        }
        else
        {
            while (input[i] && !ft_isspace(input[i]) && !is_special(input[i]))
                i++;
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
        start = i;
        if (is_special(input[i]))
        {
            if (input[i] == '>' && input[i + 1] == '>')
                i++;
            i++;
            tokens[j++] = ft_substr(input, start, i - start);
        }
        else if (input[i] == '"' || input[i] == '\'')
        {
            quote = input[i++];
            start = i;
            while (input[i] && input[i] != quote)
                i++;
            tokens[j++] = ft_substr(input, start, i - start);
            if (input[i] == quote)
                i++;
        }
        else
        {
            while (input[i] && !ft_isspace(input[i]) && !is_special(input[i]))
                i++;
            tokens[j++] = ft_substr(input, start, i - start);
        }
        skip_spaces(input, &i);
    }
    tokens[j] = NULL;
    return tokens;
}
