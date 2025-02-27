#include "../minishell.h"

int ft_echo(t_command *cmd)
{
    int i;
    int no_newline;

    i = 1;
    no_newline = 0;
    if (cmd->args && strcmp(cmd->args[1], "-n") == 0)
    {
        no_newline = 1;
        i++;
    }
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
    if (!no_newline)
        printf("\n");
    return 0;
}