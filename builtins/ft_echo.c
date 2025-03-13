#include "../minishell.h"


int ft_echo(t_command *cmd, t_shell *shell)
{
    int i = 1;
    int no_newline = 0;

    if (cmd->args[i] && strcmp(cmd->args[i], "-n") == 0)
    {
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

