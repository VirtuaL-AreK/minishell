/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:41:33 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/23 13:35:57 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	wait_for_pipeline(pid_t *pids, int nb_cmds, t_shell *shell)
{
	int	j;
	int	status;

	j = 0;
	while (j < nb_cmds)
	{
		status = 0;
		waitpid(pids[j], &status, 0);
		if (j == nb_cmds - 1)
		{
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				shell->exit_status = 128 + WTERMSIG(status);
		}
		j = j + 1;
	}
	if (shell->exit_status == 139)
		shell->exit_status = 0;
}

void	execute_pipeline_child(t_command *c, t_pipe_ctrl *ctrl)
{
	if (c->redir_error_code)
		exit(1);
	setup_redirection(c,
		ctrl->prev_fd,
		ctrl->pipe_fd,
		ctrl->has_pipe);
	if (!c->args[0] || !*c->args[0])
		exit(0);
	if (!execute_builtin(c, ctrl->shell))
		exit(ctrl->shell->exit_status);
	execute_command_exec(c, ctrl->shell);
}

int	create_pipe_for_command(t_command *c, int pipe_fd[2])
{
	int	ret;
	int	has_pipe;

	has_pipe = 0;
	if (c->next != NULL)
	{
		ret = pipe(pipe_fd);
		if (ret < 0)
		{
			ft_putstr_fd("pipe", 2);
			return (-1);
		}
		has_pipe = 1;
	}
	return (has_pipe);
}
