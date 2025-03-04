#include "../minishell.h"

// t_exit_status gexitstatus = SUCCESS;

void echo_path(char *path)
{
    int j;

    j = 0;
    while (environ[j] != NULL)
    {
        if (strncmp(environ[j], path, ft_strlen(path)) == 0)
            printf("%s", environ[j] + ft_strlen(path) + 1);
        j++;
    }
}

int ft_echo(t_command *cmd)
{
    int i;
    int no_newline;

    i = 1;
    no_newline = 0;
    if (!cmd->args || !cmd->args[1])
    {
        printf("\n");
        exit_here(SUCCESS);
        return (0);
    }
    if (cmd->args && strcmp(cmd->args[1], "-n") == 0)
    {
        no_newline = 1;
        i++;
    }
    else if (cmd->args && strcmp(cmd->args[1], "$?") == 0)
    {
        printf("%d\n", gexitstatus);
        return (0);
    }
    if (cmd->args && strncmp(cmd->args[1], "$", 1) == 0)
        echo_path(cmd->args[1] + 1);
    else
    {
        while (i < cmd->nb_arg)
        {
            char *arg = cmd->args[i];
            if (arg[0] == '"' && arg[strlen(arg) - 1] == '"')
            {
                arg[strlen(arg) - 1] = '\0';
                printf("%s", arg + 1);
            }
            else
                printf("%s", cmd->args[i]);
            if (i < cmd->nb_arg - 1)
            {
                printf(" ");
            }
            i++;
        }
    }
    if (!no_newline)
        printf("\n");
    return 0;
}