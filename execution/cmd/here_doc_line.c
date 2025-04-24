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
