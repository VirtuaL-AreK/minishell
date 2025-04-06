/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:46:10 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/06 11:50:51 by aanmazir         ###   ########.fr       */
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
			perror(c->infile);
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
			perror(c->outfile);
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

void	setup_redirection(t_command *c, int prev_fd,
		int pipe_fd[2], int has_pipe)
{
	setup_input_redirection(c, prev_fd);
	setup_output_redirection(c, has_pipe, pipe_fd);
}

void	check_directory_and_permissions(char *exec_path)
{
	struct stat	sb;
	int			ret;

	ret = stat(exec_path, &sb);
	if (ret != 0)
	{
		ft_putstr_fd("No such file or directory\n", 2);
		exit(127);
	}
	if (S_ISDIR(sb.st_mode))
	{
		fprintf(stderr, " Command not found\n");
		exit(127);
	}
	if (access(exec_path, X_OK) != 0)
	{
		perror(exec_path);
		exit(126);
	}
}
