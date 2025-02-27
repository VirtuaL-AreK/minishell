#include "../minishell.h"

extern char **environ;

int ft_env(t_command *cmd)
{
    char **env = environ;
    int i;

    if (strcmp(cmd->args[0], "env") == 0)
    {
        if (!cmd->args[1])
        {
            i = 0;
            while (env[i] != NULL)
            {
                printf("%s\n", env[i]);
                i++;
            }
        }
    }
    return (1);
}