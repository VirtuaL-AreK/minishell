#include "../minishell.h"

int ft_cd(t_command *cmd, t_shell *shell)
{
    char *path;
    char *oldpwd;
    char cwd[1024];
    int i = 0;

    if (cmd->nb_arg > 2)
    {
        ft_putstr_fd("cd: too many arguments\n", 2);
        shell->exit_status = 1;
        return 1; 
    }

    if (cmd->nb_arg < 2 || (cmd->args[1] && strcmp(cmd->args[1], "~") == 0))
    {
        // Handle no argument or `~`
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
    else if (cmd->args[1] && strncmp(cmd->args[1], "~/", 2) == 0)
    {
        // Handle `~/subdir`
        char *home = NULL;
        while (shell->env && shell->env[i])
        {
            if (ft_strncmp(shell->env[i], "HOME=", 5) == 0)
            {
                home = shell->env[i] + 5;
                break;
            }
            i++;
        }
        if (!home)
        {
            ft_putstr_fd("minishell: cd: HOME not set\n", 2);
            shell->exit_status = 1;
            return 1;
        }
        path = ft_strjoin(home, cmd->args[1] + 1); // Concatenate HOME with the rest of the path
        if (!path)
        {
            perror("malloc");
            shell->exit_status = 1;
            return 1;
        }
    }
    else
    {
        path = cmd->args[1];
    }

    oldpwd = getcwd(NULL, 0);
    if (chdir(path) != 0)
    {
        perror("cd");
        if (path != cmd->args[1]) // Free dynamically allocated path
            free(path);
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

    if (path != cmd->args[1] && path != shell->env[i] + 5) // Free dynamically allocated path
        free(path);

    shell->exit_status = 0;
    return 0;
}