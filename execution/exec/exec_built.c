/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:33:46 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/06 11:35:28 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_critical_builtin(const char *cmd)
{
	if (cmd == NULL)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	return (0);
}

int	execute_builtin_cd(t_command *cmd, t_shell *shell)
{
	ft_cd(cmd, shell);
	return (0);
}

int	execute_builtin_echo(t_command *cmd, t_shell *shell)
{
	if (cmd->args[1] != NULL)
	{
		ft_echo(cmd, shell);
	}
	else
	{
		printf("\n");
		shell->exit_status = 0;
	}
	return (0);
}

int	execute_builtin_exit(t_command *cmd, t_shell *shell)
{
	ft_exit(cmd, shell);
	return (0);
}

int	execute_builtin_export(t_command *cmd, t_shell *shell)
{
	ft_export(cmd, shell);
	return (0);
}
