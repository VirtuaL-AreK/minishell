/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:35:00 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/24 11:39:53 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	init_heredoc_signals(struct sigaction *old_sa)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = sigint_handler_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, old_sa);
	signal(SIGQUIT, SIG_IGN);
}

void	restore_heredoc_signals(struct sigaction *old_sa)
{
	sigaction(SIGINT, old_sa, NULL);
	signal(SIGQUIT, SIG_DFL);
}

int	create_heredoc_file(char *template)
{
	int	fd;

	fd = mkstemp(template);
	if (fd < 0)
	{
		ft_putstr_fd("mkstemp error\n", 2);
		return (-1);
	}
	return (fd);
}

int	read_heredoc_and_write(t_heredoc_ctx *ctx)
{
	struct sigaction	old_sa;
	char				*line;
	int					status;

	init_heredoc_signals(&old_sa);
	while (1)
	{
		line = readline("> ");
		if (!line || g_last_signal == SIGINT)
			break ;
		status = handle_heredoc_line(ctx, line);
		free(line);
		if (status != 0)
			break ;
	}
	restore_heredoc_signals(&old_sa);
	return (g_last_signal == SIGINT);
}

char	*handle_heredoc(const char *delimiter, int is_quoted, t_shell *shell)
{
	t_heredoc_ctx	ctx;
	char			*template;
	int				interrupted;
	int				fd;

	if (open_heredoc_tmp(&template, &fd) < 0)
		return (NULL);
	ctx.fd = fd;
	ctx.is_quoted = is_quoted;
	ctx.delimiter = delimiter;
	ctx.shell = shell;
	interrupted = read_heredoc_and_write(&ctx);
	if (interrupted)
		return (heredoc_on_interrupt(shell, template, fd));
	close(fd);
	return (template);
}
