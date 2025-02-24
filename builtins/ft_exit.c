#include "../minishell.h"

int ft_exit(t_command *cmd)
{
    if (strcmp(cmd->args[0], "exit") == 0)
    {
        if (!cmd->args[1])
            exit(EXIT_SUCCESS);
        else
            return (0);
    }
    return (1);
}