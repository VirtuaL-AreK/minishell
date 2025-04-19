/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:19:02 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/19 16:56:57 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	fork_pipeline_command(t_command *c, int prev_fd, int pipe_fd[2],
		int has_pipe, t_shell *shell, pid_t *pids, int *i)
{
	pid_t	pid;
	int		new_prev_fd;

	new_prev_fd = -1;
	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("fork", 2);
		return (-1);
	}
	else if (pid == 0)
		execute_pipeline_child(c, prev_fd, pipe_fd, has_pipe, shell);
	else
	{
		pids[*i] = pid;
		*i = *i + 1;
		if (prev_fd != -1)
			close(prev_fd);
		if (has_pipe)
		{
			close(pipe_fd[1]);
			new_prev_fd = pipe_fd[0];
		}
	}
	return (new_prev_fd);
}

static int	handle_pipeline_command(t_command *c, int prev_fd,
		t_shell *shell, pid_t *pids, int *i)
{
	int	pipe_fd[2];
	int	has_pipe;
	int	new_prev_fd;

	has_pipe = create_pipe_for_command(c, pipe_fd);
	if (has_pipe == -1 && c->next != NULL)
	{
		free(pids);
		shell->exit_status = 1;
		exit(1);
	}
	new_prev_fd = fork_pipeline_command(c, prev_fd, pipe_fd, has_pipe,
			shell, pids, i);
	if (new_prev_fd == -1 && has_pipe)
	{
		free(pids);
		shell->exit_status = 1;
		exit(1);
	}
	return (new_prev_fd);
}

static void	process_pipeline(t_command *cmd, t_shell *shell)
{
	int		i;
	int		nb_cmds;
	int		prev_fd;
	pid_t	*pids;

	nb_cmds = count_commands(cmd);
	pids = malloc(sizeof(pid_t) * nb_cmds);
	if (pids == NULL)
	{
		ft_putstr_fd("malloc pids", 2);
		shell->exit_status = 1;
		return ;
	}
	i = 0;
	prev_fd = -1;
	while (cmd != NULL)
	{
		prev_fd = handle_pipeline_command(cmd, prev_fd, shell, pids, &i);
		cmd = cmd->next;
	}
	wait_for_pipeline(pids, nb_cmds, shell);
	free(pids);
}

void	execute_pipeline(t_command *cmd, t_shell *shell)
{
	if (cmd != NULL && cmd->next == NULL && cmd->args[0] != NULL
		&& is_critical_builtin(cmd->args[0]))
	{
		if (cmd->redir_error_code != 0)
		{
			shell->exit_status = 1;
			return ;
		}
		execute_builtin(cmd, shell);
		return ;
	}
	process_pipeline(cmd, shell);
}
