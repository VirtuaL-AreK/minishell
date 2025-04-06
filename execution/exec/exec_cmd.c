/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:36:47 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/06 11:40:25 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	execute_command_child(char **args, char **env)
{
	char	*exec_path;
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		exec_path = args[0];
		if (access(exec_path, X_OK) != 0)
			exec_path = find_exec(args[0], env);
		if (exec_path == NULL)
		{
			printf("%s: Command not found\n", args[0]);
			exit(1);
		}
		execve(exec_path, args, env);
		ft_putstr_fd("minishell", 2);
		exit(1);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
}

void	execute_command(char **args, char **env)
{
	execute_command_child(args, env);
}

int	count_commands(t_command *cmd)
{
	int	count;

	count = 0;
	while (cmd != NULL)
	{
		count = count + 1;
		cmd = cmd->next;
	}
	return (count);
}

void	execute_command_exec(t_command *c, t_shell *shell)
{
	char	*exec_path;

	exec_path = find_exec(c->args[0], shell->env);
	if (exec_path == NULL)
	{
		ft_putstr_fd(c->args[0], 2);
		ft_putstr_fd(": Command not found\n", 2);
		exit(127);
	}
	check_directory_and_permissions(exec_path);
	execve(exec_path, c->args, shell->env);
	ft_putstr_fd(exec_path, 2);
	exit(1);
}
