/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 18:35:19 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/05 18:39:52 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*resolve_cd_path(t_command *cmd, t_shell *shell)
{
	char	*path;

	path = NULL;
	if (cmd->nb_arg > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		shell->exit_status = 1;
		return (NULL);
	}
	if (cmd->nb_arg < 2 || (cmd->args[1] && strcmp(cmd->args[1], "~") == 0))
		path = resolve_cd_path_tilde(cmd, shell);
	else if (cmd->args[1] && strcmp(cmd->args[1], "-") == 0)
		path = resolve_cd_path_dash(cmd, shell);
	else if (cmd->args[1] && strncmp(cmd->args[1], "~/", 2) == 0)
		path = resolve_cd_path_home_slash(cmd, shell);
	else
		path = cmd->args[1];
	return (path);
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

int	ft_cd(t_command *cmd, t_shell *shell)
{
	char	*path;
	char	*oldpwd;
	int		ret;

	path = resolve_cd_path(cmd, shell);
	if (!path)
		return (1);
	oldpwd = getcwd(NULL, 0);
	if (chdir(path) != 0)
	{
		ft_putstr_fd("cd", 2);
		if (cmd->args[1] && strncmp(cmd->args[1], "~/", 2) == 0)
			free(path);
		free(oldpwd);
		shell->exit_status = 1;
		return (1);
	}
	if (cmd->args[1] && strncmp(cmd->args[1], "~/", 2) == 0)
		free(path);
	ret = update_cd_env(cmd, shell, oldpwd);
	shell->path = getcwd(NULL, 0);
	if (!shell->path)
	{
		ft_putstr_fd("pwd", 2);
		shell->exit_status = 1;
		return (1);
	}
	shell->exit_status = 0;
	return (ret);
}
