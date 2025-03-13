#include "../minishell.h"

int ft_cd(t_command *cmd, t_shell *shell)
{
    char *path;
    char *oldpwd;
    char cwd[1024];

    if (cmd->nb_arg > 2)
    {
        ft_putstr_fd("cd: too many arguments\n", 2);
        shell->exit_status = 1;
        return 1; 
    }
    if (cmd->nb_arg < 2)
    {
        int i = 0;
        while (shell->env && shell->env[i])
        {
            if (ft_strncmp(shell->env[i], "HOME=", 5) == 0)
            {
                path = shell->env[i] + 5; 
                break;
            }
            i++;
        }
        if (!shell->env[i])
        {
            ft_putstr_fd("minishell: cd: HOME not set\n", 2);
            shell->exit_status = 1;
            return 1;
        }
    }
    else
        path = cmd->args[1];

    oldpwd = getcwd(NULL, 0);
    if (chdir(path) != 0)
    {
        perror("cd");
        free(oldpwd);
        shell->exit_status = 1;
        return 1;
    }

    if (oldpwd)
    {
        add_or_replace_var(shell, "OLDPWD", oldpwd);
        free(oldpwd);
    }
    if (getcwd(cwd, sizeof(cwd)))
        add_or_replace_var(shell, "PWD", cwd);

    shell->exit_status = 0;
    return 0;
}
