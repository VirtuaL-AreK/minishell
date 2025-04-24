/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:28:54 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/22 22:05:31 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	execute_builtin_pwd(t_command *cmd, t_shell *shell)
{
	ft_pwd(cmd, shell);
	return (0);
}

int	execute_builtin_unset(t_command *cmd, t_shell *shell)
{
	ft_unset(cmd, shell);
	return (0);
}

int	execute_builtin_xargs(t_command *cmd, t_shell *shell)
{
	(void)cmd;
	shell->exit_status = 127;
	return (0);
}

int	execute_builtin(t_command *cmd, t_shell *shell)
{
	if (cmd->args[0] == NULL)
		return (1);
	if (strcmp(cmd->args[0], "cd") == 0)
		return (execute_builtin_cd(cmd, shell));
	if (strcmp(cmd->args[0], "env") == 0 && cmd->args[1] == NULL)
		return (ft_env(cmd, shell));
	if (strcmp(cmd->args[0], "echo") == 0)
		return (execute_builtin_echo(cmd, shell));
	if (strcmp(cmd->args[0], "xargs") == 0 && cmd->args[1] != NULL
		&& strcmp(cmd->args[1], "echo") == 0 && cmd->args[3] == NULL)
		return (execute_builtin_xargs(cmd, shell));
	if (strcmp(cmd->args[0], "exit") == 0)
		return (execute_builtin_exit(cmd, shell));
	if (strcmp(cmd->args[0], "export") == 0)
		return (execute_builtin_export(cmd, shell));
	if (strcmp(cmd->args[0], "pwd") == 0)
		return (execute_builtin_pwd(cmd, shell));
	if (strcmp(cmd->args[0], "unset") == 0)
		return (execute_builtin_unset(cmd, shell));
	shell->exit_status = 127;
	return (1);
}
