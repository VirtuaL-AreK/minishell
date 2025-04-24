/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:46:10 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/22 22:11:07 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	setup_input_redirection(t_command *c, int prev_fd)
{
	int	in_fd;

	if (c->infile != NULL)
	{
		in_fd = open(c->infile, O_RDONLY);
		if (in_fd < 0)
		{
			ft_putstr_fd(c->infile, 2);
			exit(1);
		}
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	else if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
}

void	setup_output_redirection(t_command *c, int has_pipe, int pipe_fd[2])
{
	int	flags;
	int	out_fd;

	if (c->outfile != NULL)
	{
		flags = O_WRONLY | O_CREAT;
		if (c->append)
			flags = flags | O_APPEND;
		else
			flags = flags | O_TRUNC;
		out_fd = open(c->outfile, flags, 0644);
		if (out_fd < 0)
		{
			ft_putstr_fd(c->outfile, 2);
			exit(1);
		}
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	else if (has_pipe)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
}

void	setup_redirection(t_command *c, int prev_fd, int pipe_fd[2],
		int has_pipe)
{
	setup_input_redirection(c, prev_fd);
	setup_output_redirection(c, has_pipe, pipe_fd);
}

void	check_directory_and_permissions(const char *path)
{
	struct stat	sb;

	if (stat(path, &sb) != 0)
	{
		print_error(path);
		exit(127);
	}
	if (S_ISDIR(sb.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)path, 2);
		ft_putstr_fd(": is a directory\n", 2);
		exit(126);
	}
	if (access(path, X_OK) != 0)
	{
		print_error(path);
		exit(126);
	}
}
