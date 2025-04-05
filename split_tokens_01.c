/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens_01.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:01:56 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/05 12:15:42 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_unquoted_char(const char *line, int *i, char *buffer, int *len)
{
	buffer[*len] = line[*i];
	*len = *len + 1;
	*i = *i + 1;
}

char	*parse_one_token_merge_quotes(const char *line,
		int *i, t_token_flags *flags)
{
	char	buffer[4096];
	int		len;

	len = 0;
	while (line[*i] && !isspace((unsigned char)line[*i])
		&& !is_special_char(line[*i]))
	{
		if (line[*i] == '\'')
			process_single_quote(line, i, buffer, &len);
		else if (line[*i] == '"')
			process_double_quote(line, i, buffer, &len);
		else
			process_unquoted_char(line, i, buffer, &len);
	}
	buffer[len] = '\0';
	if (strchr(buffer, '\''))
		flags->has_sq = 1;
	if (strchr(buffer, '"'))
		flags->has_dq = 1;
	flags->should_expand = 1;
	return (ft_strdup(buffer));
}

void	process_special_char_token(const char *line, int *i, t_strlist **result)
{
	char			tmp[2];
	t_token_flags	flags;

	flags.has_sq = 0;
	flags.has_dq = 0;
	flags.should_expand = 0;
	if (line[*i] == '<' && line[*i + 1] == '<')
	{
		add_strlist(result, "<<", flags);
		*i = *i + 2;
	}
	else if (line[*i] == '>' && line[*i + 1] == '>')
	{
		add_strlist(result, ">>", flags);
		*i = *i + 2;
	}
	else
	{
		tmp[0] = line[*i];
		tmp[1] = '\0';
		add_strlist(result, tmp, flags);
		*i = *i + 1;
	}
}
