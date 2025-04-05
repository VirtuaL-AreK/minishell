/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens_0.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:57:59 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/05 12:01:07 by aanmazir         ###   ########.fr       */
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

void	process_single_quote(const char *line, int *i, char *buffer, int *len)
{
	buffer[*len] = line[*i];
	*len = *len + 1;
	*i = *i + 1;
	while (line[*i] && line[*i] != '\'')
	{
		buffer[*len] = line[*i];
		*len = *len + 1;
		*i = *i + 1;
	}
	if (line[*i] == '\'')
	{
		buffer[*len] = line[*i];
		*len = *len + 1;
		*i = *i + 1;
	}
}

void	process_double_quote(const char *line, int *i, char *buffer, int *len)
{
	buffer[*len] = line[*i];
	*len = *len + 1;
	*i = *i + 1;
	while (line[*i] && line[*i] != '"')
	{
		buffer[*len] = line[*i];
		*len = *len + 1;
		*i = *i + 1;
	}
	if (line[*i] == '"')
	{
		buffer[*len] = line[*i];
		*len = *len + 1;
		*i = *i + 1;
	}
}
