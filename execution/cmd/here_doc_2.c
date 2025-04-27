/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:41:36 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/26 18:15:39 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	try_open_with_index(const char *base,
						int i,
						char **out_template,
						int *out_fd)
{
	char	*idx;
	char	*path;
	int		fd;

	idx = ft_itoa(i);
	if (!idx)
		return (-1);
	path = ft_strjoin(base, idx);
	free(idx);
	if (!path)
		return (-1);
	fd = -1;
	if (access(path, F_OK) == -1)
		fd = open(path, O_CREAT | O_EXCL | O_RDWR, 0600);
	if (fd < 0)
	{
		free(path);
		return (0);
	}
	*out_template = path;
	*out_fd = fd;
	return (1);
}

int	find_and_open_template(char *base,
							char **out_template,
							int *out_fd)
{
	int	i;
	int	res;

	i = 0;
	while (i < 1000)
	{
		res = try_open_with_index(base, i, out_template, out_fd);
		if (res == 1)
			return (0);
		if (res == -1)
			return (-1);
		i++;
	}
	return (-1);
}

int	open_heredoc_tmp(char **out_template, int *out_fd)
{
	char	*base;
	int		ret;

	base = ft_strdup("/tmp/minishell_heredoc_");
	if (!base)
		return (-1);
	ret = find_and_open_template(base, out_template, out_fd);
	free(base);
	return (ret);
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
