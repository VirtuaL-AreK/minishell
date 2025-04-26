/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:35:00 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/25 20:06:27 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// void	generate_random_chars(char *str, size_t len)
// {
// 	size_t				i;
// 	static const char	*chars;

// 	i = 0;
// 	chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
// 	while (i < len)
// 	{
// 		str[i] = chars[rand() % (sizeof(chars) - 1)];
// 		i++;
// 	}
// 	str[len] = '\0';
// }

// int	ft_mkstemp(char *template)
// {
// 	char	*xx_pos;
// 	int		attempt;
// 	int		fd;

// 	if (template == NULL || ft_strlen(template) < 6)
// 		return (-1);
// 	xx_pos = strstr(template, "XXXXXX");
// 	if (xx_pos == NULL)
// 		return (-1);
// 	srand((unsigned int)time(NULL));
// 	attempt = 0;
// 	while (attempt < 10000)
// 	{
// 		generate_random_chars(xx_pos, 6);
// 		fd = open(template, O_RDWR | O_CREAT | O_EXCL, 0600);
// 		if (fd != -1)
// 			return (fd);
// 		attempt++;
// 	}
// 	return (-1);
// }

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

// int	create_heredoc_file(char *template)
// {
// 	int	fd;

// 	fd = ft_mkstemp(template);
// 	if (fd < 0)
// 	{
// 		ft_putstr_fd("mkstemp error\n", 2);
// 		return (-1);
// 	}
// 	return (fd);
// }
