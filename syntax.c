#include "minishell.h"

int check_unclosed_quotes(const char *input, t_shell *shell)
{
    int single_quote = 0;
    int double_quote = 0;
    int i = 0;

    while (input[i])
    {
        if (input[i] == '\'' && double_quote == 0)
            single_quote = !single_quote;
        else if (input[i] == '"' && single_quote == 0)
            double_quote = !double_quote;
        i++;
    }
    if (single_quote || double_quote)
    {
        printf("Syntax error: unclosed quote\n");
		shell->exit_status = 2;
        return 1;
    }
    return 0;
}
