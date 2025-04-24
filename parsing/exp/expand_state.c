/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_state.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:59:37 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/24 15:00:29 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	init_expand_state(t_expand_state *st, int init_cap)
{
	st->buffer = (char *)malloc(init_cap);
	if (!st->buffer)
		return (-1);
	st->capacity = init_cap;
	st->idx = 0;
	st->in_sq = 0;
	st->in_dq = 0;
	return (0);
}

int	expand_buffer_if_needed(t_expand_state *st, int needed)
{
	char	*new_buf;
	int		new_cap;

	if (st->idx + needed < st->capacity)
		return (0);
	new_cap = st->capacity;
	while (new_cap < st->idx + needed)
		new_cap *= 2;
	new_buf = (char *)malloc(new_cap);
	if (!new_buf)
		return (-1);
	ft_memcpy(new_buf, st->buffer, st->idx);
	free(st->buffer);
	st->buffer = new_buf;
	st->capacity = new_cap;
	return (0);
}

int	expand_add_char(t_expand_state *st, char c)
{
	if (expand_buffer_if_needed(st, 1 + 1) < 0)
		return (-1);
	st->buffer[st->idx] = c;
	st->idx++;
	return (0);
}

int	expand_add_string(t_expand_state *st, const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (expand_add_char(st, s[i]) < 0)
			return (-1);
		i++;
	}
	return (0);
}

int	process_dollar_branch(const char *str, int i,
		t_expand_state *state, t_shell *shell)
{
	if (str[i + 1] == '\'' || str[i + 1] == '"')
		handle_dollar_quoted(str, &i, state, shell);
	else
		handle_variable(str, &i, state, shell);
	return (i);
}
