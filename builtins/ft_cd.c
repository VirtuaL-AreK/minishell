/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 18:35:19 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/24 10:27:42 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_oldpwd(void)
{
	char	*oldpwd;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		oldpwd = ft_strdup("");
	return (oldpwd);
}

static const char	*cd_error_msg(int err)
{
	if (err == ENOENT)
		return ("No such file or directory");
	if (err == ENOTDIR)
		return ("Not a directory");
	if (err == EACCES)
		return ("Permission denied");
	return ("Error");
}

static int	perform_cd(const char *path, t_command *cmd,
		t_shell *shell, char **oldpwd)
{
	const char	*msg;
	char		cwd[1024];

	if (chdir(path) != 0)
	{
		msg = cd_error_msg(errno);
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd((char *)path, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd((char *)msg, 2);
		free(*oldpwd);
		shell->exit_status = 1;
		return (1);
	}
	if (cmd->args[1] && ft_strncmp(cmd->args[1], "-", 1) == 0)
		if (getcwd(cwd, sizeof(cwd)))
			ft_putendl_fd(cwd, 1);
	add_or_replace_var(shell, "OLDPWD", *oldpwd);
	free(*oldpwd);
	if (getcwd(cwd, sizeof(cwd)))
		add_or_replace_var(shell, "PWD", cwd);
	shell->path = getcwd(NULL, 0);
	shell->exit_status = 0;
	return (0);
}

static char	*check_command(t_command *cmd, t_shell *shell)
{
	if (cmd->nb_arg >= 2
		&& cmd->args[1][0] == '-'
		&& ft_strncmp(cmd->args[1], "-", 1) != 0
		&& ft_strncmp(cmd->args[1], "--", 2) != 0)
	{
		ft_putstr_fd("bash: cd: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd(": invalid option\ncd: usage: cd [dir]\n", 2);
		shell->exit_status = 2;
		return (NULL);
	}
	return (resolve_cd_path(cmd, shell));
}

int	ft_cd(t_command *cmd, t_shell *shell)
{
	char	*path;
	char	*oldpwd;

	path = check_command(cmd, shell);
	if (!path)
		return (1);
	oldpwd = get_oldpwd();
	path = resolve_cd_path(cmd, shell);
	if (!path)
	{
		free(oldpwd);
		return (1);
	}
	return (perform_cd(path, cmd, shell, &oldpwd));
}
