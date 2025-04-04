#include "../minishell.h"

static char *get_home_from_env(t_shell *shell)
{
    int i;
    char *home;

    i = 0;
    home = NULL;
    while (shell->env && shell->env[i])
    {
        if (ft_strncmp(shell->env[i], "HOME=", 5) == 0)
        {
            home = shell->env[i] + 5;
            break;
        }
        i = i + 1;
    }
    return home;
}

static char *get_oldpwd_from_env(t_shell *shell)
{
    int i;
    char *oldpwd;

    i = 0;
    oldpwd = NULL;
    while (shell->env && shell->env[i])
    {
        if (ft_strncmp(shell->env[i], "OLDPWD=", 7) == 0)
        {
            oldpwd = shell->env[i] + 7;
            break;
        }
        i = i + 1;
    }
    return oldpwd;
}

static char *resolve_cd_path_tilde(t_command *cmd, t_shell *shell)
{
    char *home;
    
	(void)cmd;
    home = get_home_from_env(shell);
    if (!home)
    {
        ft_putstr_fd("minishell: cd: HOME not set\n", 2);
        shell->exit_status = 1;
        return NULL;
    }
    return home;
}

static char *resolve_cd_path_dash(t_command *cmd, t_shell *shell)
{
    char *path;
    
	(void)cmd;
    path = get_oldpwd_from_env(shell);
    if (!path)
    {
        ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
        shell->exit_status = 1;
        return NULL;
    }
    return path;
}

static char *resolve_cd_path_home_slash(t_command *cmd, t_shell *shell)
{
    char *home;
    char *path;
    
    home = get_home_from_env(shell);
    if (!home)
    {
        ft_putstr_fd("minishell: cd: HOME not set\n", 2);
        shell->exit_status = 1;
        return NULL;
    }
    path = ft_strjoin(home, cmd->args[1] + 1);
    if (!path)
    {
        perror("malloc");
        shell->exit_status = 1;
        return NULL;
    }
    return path;
}

static char *resolve_cd_path(t_command *cmd, t_shell *shell)
{
    char *path;
    
    path = NULL;
    if (cmd->nb_arg > 2)
    {
        ft_putstr_fd("cd: too many arguments\n", 2);
        shell->exit_status = 1;
        return NULL;
    }
    if (cmd->nb_arg < 2 || (cmd->args[1] && strcmp(cmd->args[1], "~") == 0))
        path = resolve_cd_path_tilde(cmd, shell);
    else if (cmd->args[1] && strcmp(cmd->args[1], "-") == 0)
        path = resolve_cd_path_dash(cmd, shell);
    else if (cmd->args[1] && strncmp(cmd->args[1], "~/", 2) == 0)
        path = resolve_cd_path_home_slash(cmd, shell);
    else
        path = cmd->args[1];
    return path;
}


static int update_cd_env(t_command *cmd, t_shell *shell, char *oldpwd)
{
    char cwd[1024];

    if (cmd->args[1] && strcmp(cmd->args[1], "-") == 0)
    {
        if (getcwd(cwd, sizeof(cwd)))
            ft_putendl_fd(cwd, 1);
    }
    if (oldpwd)
    {
        add_or_replace_var(shell, "OLDPWD", oldpwd);
        free(oldpwd);
    }
    if (getcwd(cwd, sizeof(cwd)))
        add_or_replace_var(shell, "PWD", cwd);
    return 0;
}

int ft_cd(t_command *cmd, t_shell *shell)
{
    char *path;
    char *oldpwd;
    int ret;

    path = resolve_cd_path(cmd, shell);
    if (!path)
        return 1;
    oldpwd = getcwd(NULL, 0);
    if (chdir(path) != 0)
    {
        perror("cd");
        if (cmd->args[1] && strncmp(cmd->args[1], "~/", 2) == 0)
            free(path);
        free(oldpwd);
        shell->exit_status = 1;
        return 1;
    }
    if (cmd->args[1] && strncmp(cmd->args[1], "~/", 2) == 0)
        free(path);
    ret = update_cd_env(cmd, shell, oldpwd);
    shell->exit_status = 0;
    return ret;
}
