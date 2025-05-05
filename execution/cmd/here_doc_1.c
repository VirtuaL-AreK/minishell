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
