/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 18:27:12 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/05 18:31:51 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_home_from_env(t_shell *shell)
{
	int		i;
	char	*home;

	i = 0;
	home = NULL;
	while (shell->env && shell->env[i])
	{
		if (ft_strncmp(shell->env[i], "HOME=", 5) == 0)
		{
			home = shell->env[i] + 5;
			break ;
		}
		i = i + 1;
	}
	return (home);
}

char	*get_oldpwd_from_env(t_shell *shell)
{
	int		i;
	char	*oldpwd;

	i = 0;
	oldpwd = NULL;
	while (shell->env && shell->env[i])
	{
		if (ft_strncmp(shell->env[i], "OLDPWD=", 7) == 0)
		{
			oldpwd = shell->env[i] + 7;
			break ;
		}
		i = i + 1;
	}
	return (oldpwd);
}

char	*resolve_cd_path_tilde(t_command *cmd, t_shell *shell)
{
	char	*home;

	(void)cmd;
	home = get_home_from_env(shell);
	if (!home)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		shell->exit_status = 1;
		return (NULL);
	}
	return (home);
}

char	*resolve_cd_path_dash(t_command *cmd, t_shell *shell)
{
	char	*path;

	(void)cmd;
	path = get_oldpwd_from_env(shell);
	if (!path)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		shell->exit_status = 1;
		return (NULL);
	}
	return (path);
}

char	*resolve_cd_path_home_slash(t_command *cmd, t_shell *shell)
{
	char	*home;
	char	*path;

	home = get_home_from_env(shell);
	if (!home)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		shell->exit_status = 1;
		return (NULL);
	}
	path = ft_strjoin(home, cmd->args[1] + 1);
	if (!path)
	{
		perror("malloc");
		shell->exit_status = 1;
		return (NULL);
	}
	return (path);
}
