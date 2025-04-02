#include "../minishell.h"


int ft_echo(t_command *cmd, t_shell *shell)
{
    int i = 1;
    int no_newline = 0;

    while (cmd->args[i] && cmd->args[i][0] == '-' && cmd->args[i][1])
    {
        int j = 1;
        int valid = 1;
        while (cmd->args[i][j])
        {
            if (cmd->args[i][j] != 'n')
            {
                valid = 0;
                break;
            }
            j++;
        }
        if (!valid)
            break;
        no_newline = 1;
        i++;
    }

    while (i < cmd->nb_arg)
    {
        printf("%s", cmd->args[i]);
        if (i < cmd->nb_arg - 1)
            putchar(' ');
        i++;
    }
    if (!no_newline)
        putchar('\n');

    shell->exit_status = 0;
    return 0;
}


