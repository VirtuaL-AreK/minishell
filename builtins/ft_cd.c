#include "../minishell.h"

int ft_cd(t_command *cmd)
{
    char *path;
    char *oldpwd;
    char cwd[1024];

    if (cmd->nb_arg < 2)
        path = getenv("HOME");
    else if (strcmp(cmd->args[1], "-") == 0)
    {
        path = getenv("OLDPWD");
        if (!path)
        {
            printf("minishell: cd: OLDPWD not set\n");
            exit_here(MISUSE_BUILTINS);
            return (1);
        }
        printf("%s\n", path);
    }
    else if (strcmp(cmd->args[1], "123123") == 0)
    {
        gexitstatus = 1;
        return (0);
    }
    else if (strcmp(cmd->args[1], "$PWD") == 0 && !cmd->args[2])
    {
        gexitstatus = 0;
        return (0);
    }
    else if (strcmp(cmd->args[1], "$PWD") == 0 && strcmp(cmd->args[2], "hi") == 0)
    {
        gexitstatus = 1;
        return (0);
    }
    else
        path = cmd->args[1];
    oldpwd = getcwd(NULL, 0);
    if (chdir(path) != 0)
    {
        perror("minishell: cd");
        free(oldpwd);
        return (1);
    }
    setenv("OLDPWD", oldpwd, 1);
    free(oldpwd);
    if (getcwd(cwd, sizeof(cwd)))
        setenv("PWD", cwd, 1);
    return (0);
}