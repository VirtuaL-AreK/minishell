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

// int	open_heredoc_tmp(char **out_template, int *out_fd)
// {
// 	*out_template = ft_strdup("/tmp/minishell_heredoc_XXXXXX");
// 	if (!*out_template)
// 		return (-1);
// 	*out_fd = mkstemp(*out_template);
// 	if (*out_fd < 0)
// 	{
// 		ft_putstr_fd("mkstemp error\n", 2);
// 		free(*out_template);
// 		return (-1);
// 	}
// 	return (0);
// }

// int	open_heredoc_tmp(char **out_template, int *out_fd)
// {
// 	char	*base;
// 	char	*idx;
// 	char	*path;
// 	int		fd;
// 	int		i;

// 	base = ft_strdup("/tmp/minishell_heredoc_");
// 	if (!base)
// 		return (-1);
// 	i = 0;
// 	while (i < 1000)
// 	{
// 		idx = ft_itoa(i);
// 		path = ft_strjoin(base, idx);
// 		free(idx);
// 		if (!path)
// 			break ;
// 		if (access(path, F_OK) == -1)
// 		{
// 			fd = open(path, O_CREAT | O_EXCL | O_RDWR, 0600);
// 			if (fd >= 0)
// 			{
// 				free(base);
// 				*out_template = path;
// 				*out_fd = fd;
// 				return (0);
// 			}
// 		}
// 		free(path);
// 		i++;
// 	}
// 	free(base);
// 	return (-1);
// }

static int try_open_with_index(const char *base,
                               int i,
                               char **out_template,
                               int *out_fd)
{
    char *idx;
    char *path;
    int   fd;

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
    *out_fd       = fd;
    return (1);
}

static int find_and_open_template(char *base,
                                  char **out_template,
                                  int *out_fd)
{
    int i;
    int res;

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

int open_heredoc_tmp(char **out_template, int *out_fd)
{
    char *base;
    int   ret;

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
