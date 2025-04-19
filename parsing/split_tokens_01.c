/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens_01.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:01:56 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/18 21:02:26 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*parse_one_token_merge_quotes(const char *line,
	int *i, t_token_flags *flags)
{
	char	*buf;
	int		capacity;
	int		len;
	int		ret;

	capacity = 64;
	buf = malloc(capacity);
	if (!buf)
		return (NULL);
	len = 0;

	while (line[*i] && !isspace((unsigned char)line[*i])
		&& !is_special_char(line[*i]))
	{
		if (line[*i] == '\'')
			ret = process_single_quote(line, i, &buf, &len, &capacity);
		else if (line[*i] == '"')
			ret = process_double_quote(line, i, &buf, &len, &capacity);
		else
			ret = process_unquoted_char(line, i, &buf, &len, &capacity);
		if (ret < 0)
		{
			free(buf);
			return (NULL);
		}
	}

	if (append_char(&buf, &len, &capacity, '\0') < 0)
	{
		free(buf);
		return (NULL);
	}

	if (strchr(buf, '\''))
		flags->has_sq = 1;
	if (strchr(buf, '"'))
		flags->has_dq = 1;
	flags->should_expand = 1;

	return (buf);
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
