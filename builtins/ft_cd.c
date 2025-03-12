#include "../minishell.h"

int ft_cd(t_command *cmd)
{
    char *path;
    char *oldpwd;
    char cwd[1024];

    if (cmd->nb_arg > 2)
    {
        ft_putstr_fd("cd: too many arguments\n", 2);
        gexitstatus = 1;
        return 1; 
    }

    if (cmd->nb_arg < 2)
    {
        path = getenv("HOME");
        if (!path)
        {
            ft_putstr_fd("minishell: cd: HOME not set\n", 2);
            gexitstatus = 1;
            return 1;
        }
    }
    else if (strcmp(cmd->args[1], "-") == 0)
    {
        path = getenv("OLDPWD");
        if (!path)
        {
            ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
            gexitstatus = 1;
            return 1;
        }
        printf("%s\n", path);
    }
    else
    {
        path = cmd->args[1];
    }

    oldpwd = getcwd(NULL, 0);
    if (chdir(path) != 0)
    {
        perror("minishell: cd");
        free(oldpwd);
        gexitstatus = 1;
        return 1;
    }

    setenv("OLDPWD", oldpwd, 1);
    free(oldpwd);
    if (getcwd(cwd, sizeof(cwd)))
        setenv("PWD", cwd, 1);

    gexitstatus = 0;
    return 0;
}
