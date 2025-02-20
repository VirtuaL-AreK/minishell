#include "minishell.h"

int is_special_char(char c)
{
    char special_char[] = {
        '|', '<', '>', '$', '\\', '"',
        '/', ' ', '\t', '?', '&', 
        '(', ')', '*', '.', '-'
    };
    int i;
    int len;

    i = 0;
    len = sizeof(special_char) / sizeof(special_char[0]);
    while (i < len)
    {
        if (c == (char)special_char[i])
            return (1);
        i++;
    }
    return (0);
}

int syntax_verification(char **a)
{
    int i;
    int j;

    i = 0;
    while (a[i] != NULL)
    {
        j = 0;
        while (a[i][j] != '\0')
        {
            if (!(is_special_char(a[i][j]) || ft_isalnum(a[i][j])))
                return (0);
            j++;
        }
        i++;
    }
    return (1);
}

// && a[i] == '<' && a[i] == '>' && a[i] == '$'
// && a[i] == '\\' && a[i] == '"' && a[i] == '/' && a[i] == ' '
// && a[i] == '\t' && a[i] == '?' && a[i] == '&'
// && a[i] == '(' && a[i] == ')' && a[i] == '*'