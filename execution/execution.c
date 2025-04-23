/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:19:02 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/23 13:36:55 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int fork_pipeline_command(t_command *c, t_pipe_ctrl *ctrl)
{
    pid_t pid;
    int   new_prev_fd = -1;

    pid = fork();
    if (pid < 0)
    {
        ft_putstr_fd("fork\n", 2);
        return (-1);
    }
    if (pid == 0)
        execute_pipeline_child(c, ctrl);
    else
    {
        ctrl->pids[*ctrl->idx] = pid;
        (*ctrl->idx)++;
        if (ctrl->prev_fd != -1)
            close(ctrl->prev_fd);
        if (ctrl->has_pipe)
        {
            close(ctrl->pipe_fd[1]);
            new_prev_fd = ctrl->pipe_fd[0];
        }
    }
    return (new_prev_fd);
}

int handle_pipeline_command(t_command *c, t_pipe_ctrl *ctrl)
{
    int new_prev_fd;

    ctrl->has_pipe = create_pipe_for_command(c, ctrl->pipe_fd);
    if (ctrl->has_pipe < 0 && c->next)
    {
        free(ctrl->pids);
        ctrl->shell->exit_status = 1;
        exit(1);
    }
    new_prev_fd = fork_pipeline_command(c, ctrl);
    if (new_prev_fd < 0 && ctrl->has_pipe)
    {
        free(ctrl->pids);
        ctrl->shell->exit_status = 1;
        exit(1);
    }
    return (new_prev_fd);
}

void process_pipeline(t_command *cmd, t_shell *shell)
{
    t_pipe_ctrl ctrl;
    int         nb;
    int         idx;

    nb = count_commands(cmd);
    ctrl.pids = malloc(sizeof(pid_t) * nb);
    if (!ctrl.pids)
    {
        ft_putstr_fd("malloc pids\n", 2);
        shell->exit_status = 1;
        return;
    }
    idx = 0;
    ctrl.shell   = shell;
    ctrl.idx     = &idx;
    ctrl.prev_fd = -1;
    while (cmd)
    {
        ctrl.prev_fd = handle_pipeline_command(cmd, &ctrl);
        cmd = cmd->next;
    }
    wait_for_pipeline(ctrl.pids, nb, shell);
    free(ctrl.pids);
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
