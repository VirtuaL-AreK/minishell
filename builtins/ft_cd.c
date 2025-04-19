/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 18:35:19 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/19 17:56:58 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char *resolve_cd_path(t_command *cmd, t_shell *shell)
{
	char *arg1;

	if (cmd->nb_arg == 1)
		return resolve_cd_path_tilde(cmd, shell);

	arg1 = cmd->args[1];

	if (strcmp(arg1, "--") == 0)
	{
		if (cmd->nb_arg == 2)
			return resolve_cd_path_tilde(cmd, shell);
		if (cmd->nb_arg == 3)
			return cmd->args[2];
		ft_putstr_fd("cd: too many arguments\n", 2);
		shell->exit_status = 1;
		return NULL;
	}

	if (strcmp(arg1, "-") == 0 && cmd->nb_arg == 2)
		return resolve_cd_path_dash(cmd, shell);

	if (ft_strncmp(arg1, "~/", 2) == 0)
		return resolve_cd_path_home_slash(cmd, shell);

	if (cmd->nb_arg > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		shell->exit_status = 1;
		return NULL;
	}
	return arg1; 
}

static int	update_cd_env(t_command *cmd, t_shell *shell, char *oldpwd)
{
	char	cwd[1024];

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
	return (0);
}

static char *check_command(t_command *cmd, t_shell *shell)
{
	if (cmd->nb_arg >= 2 &&
		cmd->args[1][0] == '-' &&
		strcmp(cmd->args[1], "-")  != 0 &&
		strcmp(cmd->args[1], "--") != 0)
	{
		ft_putstr_fd("bash: cd: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd(": invalid option\ncd: usage: cd [dir]\n", 2);
		shell->exit_status = 2;
		return NULL;
	}
	return resolve_cd_path(cmd, shell);
}


int ft_cd(t_command *cmd, t_shell *shell)
{
    char  *path;
    char  *oldpwd;
    int    ret;
    char *err_msg;

    path = check_command(cmd, shell);
    if (!path)
        return 1;

    oldpwd = getcwd(NULL, 0);
    if (!oldpwd)
        oldpwd = ft_strdup("");

    if (chdir(path) != 0)
    {
        if (errno == ENOENT)
            err_msg = "No such file or directory";
        else if (errno == ENOTDIR)
            err_msg = "Not a directory";
        else if (errno == EACCES)
            err_msg = "Permission denied";
        else
            err_msg = "Error";

        ft_putstr_fd("minishell: cd: ", 2);
        ft_putstr_fd(path, 2);
        ft_putstr_fd(": ", 2);
        ft_putendl_fd(err_msg, 2);

        free(oldpwd);
        shell->exit_status = 1;
        return 1;
    }

    ret = update_cd_env(cmd, shell, oldpwd);
    shell->path = getcwd(NULL, 0);
    shell->exit_status = 0;
    return ret;
}
