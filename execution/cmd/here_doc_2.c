/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:41:36 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/24 11:45:44 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	open_heredoc_tmp(char **out_template, int *out_fd)
{
	*out_template = ft_strdup("/tmp/minishell_heredoc_XXXXXX");
	if (!*out_template)
		return (-1);
	*out_fd = mkstemp(*out_template);
	if (*out_fd < 0)
	{
		ft_putstr_fd("mkstemp error\n", 2);
		free(*out_template);
		return (-1);
	}
	return (0);
}

char	*heredoc_on_interrupt(t_shell *shell, char *template, int fd)
{
	close(fd);
	shell->exit_status = 130;
	shell->heredoc_interrupted = 1;
	g_last_signal = 0;
	free(template);
	return (NULL);
}

void	write_heredoc_line(t_heredoc_ctx *ctx, const char *s)
{
	write(ctx->fd, s, ft_strlen(s));
	write(ctx->fd, "\n", 1);
}

int	handle_heredoc_line(t_heredoc_ctx *ctx, char *line)
{
	char	*cmp;
	int		done;

	if (ctx->is_quoted)
		cmp = line;
	else
		cmp = expand_heredoc_line(line, ctx->shell);
	done = (strcmp(cmp, ctx->delimiter) == 0);
	if (done)
	{
		if (!ctx->is_quoted)
			free(cmp);
		return (1);
	}
	if (ctx->is_quoted)
		write_heredoc_line(ctx, line);
	else
	{
		write_heredoc_line(ctx, cmp);
		free(cmp);
	}
	return (0);
}

void	sigint_handler_heredoc(int sig)
{
	char	nl;

	(void)sig;
	g_last_signal = SIGINT;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_done = 1;
	{
		nl = '\n';
		ioctl(STDIN_FILENO, TIOCSTI, &nl);
	}
}
