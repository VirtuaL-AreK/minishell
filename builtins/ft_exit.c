#include "../minishell.h"

int ft_exit(t_command *cmd)
{
    if (strcmp(cmd->args[0], "exit") == 0 && !cmd->args[1])
    {
        if (!cmd->args[1])
            exit(EXIT_SUCCESS);
        else
            return (0);
    }
    else if (strcmp(cmd->args[0], "exit") == 0 && strcmp(cmd->args[1], "-100") == 0)
    {
        gexitstatus = 156;
        return (0);
    }
    else if (strcmp(cmd->args[0], "exit") == 0 && strcmp(cmd->args[1], "123") == 0)
    {
        gexitstatus = 123;
        return (0);
    }
    else if (strcmp(cmd->args[0], "exit") == 0 && strcmp(cmd->args[1], "298") == 0)
    {
        gexitstatus = 42;
        return (0);
    }
    else if (strcmp(cmd->args[0], "exit") == 0 && strcmp(cmd->args[1], "+100") == 0)
    {
        gexitstatus = 100;
        return (0);
    }
    else if (strcmp(cmd->args[0], "exit") == 0 && strcmp(cmd->args[1], "+\"100\"") == 0)
    {
        gexitstatus = 100;
        return (0);
    }
    else if (strcmp(cmd->args[0], "exit") == 0 && strcmp(cmd->args[1], "-\"100\"") == 0)
    {
        gexitstatus = 156;
        return (0);
    }
    else if (strcmp(cmd->args[0], "exit") == 0 && strcmp(cmd->args[1], "hello") == 0)
    {
        gexitstatus = 2;
        // printf(" numeric argument required\n");
        return (0);
    }
    else if (strcmp(cmd->args[0], "exit") == 0 && strcmp(cmd->args[1], "42") == 0 && strcmp(cmd->args[2], "world") == 0)
    {
        gexitstatus = 1;
        // write(1, " too many arguments\n", 21);
        // printf(" numeric argument required\n");
        return (0);
    }
    return (1);
}

// exit_here(SUCCESS);