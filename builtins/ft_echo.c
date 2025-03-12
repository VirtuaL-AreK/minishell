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

    gexitstatus = 0;
    return 0;
}


