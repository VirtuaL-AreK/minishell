/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:17:32 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/24 13:18:42 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

char	*expand_heredoc_line(const char *line, t_shell *shell)
{
	char	*result;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
			i = process_dollar_in_heredoc(line, i, &result, shell);
		else
		{
			append_char_result(line[i], &result);
			i++;
		}
	}
	return (result);
}

int	handle_heredoc_line(t_heredoc_ctx *ctx, char *line)
{
	char	*cmp;
	int		done;

	if (ctx->is_quoted)
		cmp = line;
	else
		cmp = expand_heredoc_line(line, ctx->shell);
	done = (ft_strcmp(cmp, ctx->delimiter) == 0);
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
