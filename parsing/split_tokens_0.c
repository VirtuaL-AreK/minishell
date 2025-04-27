/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens_0.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:57:59 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/24 12:27:00 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_special_char(int c)
{
	return (c == '|' || c == '<' || c == '>');
}

void	skip_spaces(const char *line, int *i)
{
	while (line[*i] && ft_isspace((unsigned char)line[*i]))
		(*i)++;
}

void	add_strlist(t_strlist **head, const char *value, t_token_flags flags)
{
	t_strlist	*node;
	t_strlist	*tmp;

	node = malloc(sizeof(*node));
	node->str = ft_strdup(value);
	node->has_single_quote = flags.has_sq;
	node->has_double_quote = flags.has_dq;
	node->should_expand = flags.should_expand;
	node->next = NULL;
	if (!*head)
		*head = node;
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
	ft_memcpy(new_buf, *buf, *len);
	free(*buf);
	*buf = new_buf;
	*cap = new_cap;
	(*buf)[*len] = c;
	(*len)++;
	return (0);
}

int	append_str(char **buf, int *len, int *cap, const char *s)
{
	int	j;

	j = 0;
	while (s[j])
	{
		if (append_char(buf, len, cap, s[j]) < 0)
			return (-1);
		j++;
	}
	return (0);
}
