/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens_0.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:57:59 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/10 19:30:20 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_special_char(int c)
{
	return (c == '|' || c == '<' || c == '>');
}

void	skip_spaces(const char *line, int *i)
{
	while (line[*i] && isspace((unsigned char)line[*i]))
		(*i)++;
}

void	add_strlist(t_strlist **head, const char *value, t_token_flags flags)
{
	t_strlist	*node;
	t_strlist	*tmp;

	node = malloc(sizeof(*node));
	node->str = strdup(value);
	node->has_single_quote = flags.has_sq;
	node->has_double_quote = flags.has_dq;
	node->should_expand = flags.should_expand;
	node->next = NULL;
	if (!*head)
	{
		*head = node;
	}
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = node;
	}
}

int	append_char(char **buf, int *len, int *cap, char c)
{
	char	*new_buf;
	int		new_cap;

	if (*len + 1 < *cap)
	{
		(*buf)[*len] = c;
		(*len)++;
		return (0);
	}
	new_cap = (*cap) * 2;
	new_buf = malloc(new_cap);
	if (!new_buf)
		return (-1);
	memcpy(new_buf, *buf, *len);
	free(*buf);
	*buf = new_buf;
	*cap = new_cap;
	(*buf)[*len] = c;
	(*len)++;
	return (0);
}

int	append_str(char **buf, int *len, int *cap, const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (append_char(buf, len, cap, s[i]) < 0)
			return (-1);
		i++;
	}
	return (0);
}

int	process_single_quote(const char *line, int *i,
	char **buf, int *len, int *cap)
{
	if (append_char(buf, len, cap, line[*i]) < 0)
		return (-1);
	(*i)++;
	while (line[*i] && line[*i] != '\'')
	{
		if (append_char(buf, len, cap, line[*i]) < 0)
			return (-1);
		(*i)++;
	}
	if (line[*i] == '\'')
	{
		if (append_char(buf, len, cap, line[*i]) < 0)
			return (-1);
		(*i)++;
	}
	return (0);
}

/*
** process_double_quote: idem pour les ".
*/
int	process_double_quote(const char *line, int *i, char **buf, int *len, int *cap)
{
	if (append_char(buf, len, cap, line[*i]) < 0)
		return (-1);
	(*i)++;
	while (line[*i] && line[*i] != '"')
	{
		if (append_char(buf, len, cap, line[*i]) < 0)
			return (-1);
		(*i)++;
	}
	if (line[*i] == '"')
	{
		if (append_char(buf, len, cap, line[*i]) < 0)
			return (-1);
		(*i)++;
	}
	return (0);
}

int	process_unquoted_char(const char *line, int *i, char **buf, int *len, int *cap)
{
	if (append_char(buf, len, cap, line[*i]) < 0)
		return (-1);
	(*i)++;
	return (0);
}