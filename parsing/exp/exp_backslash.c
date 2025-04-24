/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_backslash.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:55:15 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/24 14:55:17 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	process_backslash_branch(const char *str, int i, t_expand_state *state)
{
	i = i + 1;
	if (str[i])
	{
		state->buffer[state->idx] = str[i];
		state->idx = state->idx + 1;
		return (i + 1);
	}
	return (i);
}

int	handle_dollar_expand(const char *str,
							int *i,
							t_expand_state *st,
							t_shell *shell)
{
	if (handle_variable(str, i, st, shell) < 0)
		return (-1);
	return (*i);
}

int	handle_quote_toggle(const char *str,
						int i,
						t_expand_state *st)
{
	if (str[i] == '\'' && !st->in_dq)
	{
		st->in_sq = !st->in_sq;
		return (i + 1);
	}
	if (str[i] == '"' && !st->in_sq)
	{
		st->in_dq = !st->in_dq;
		return (i + 1);
	}
	return (-1);
}

int	handle_backslash_expand(const char *str,
							int i,
							t_expand_state *st)
{
	i++;
	if (str[i])
	{
		if (expand_add_char(st, str[i]) < 0)
			return (-1);
		i++;
	}
	return (i);
}

int	handle_regular_char(const char *str,
						int i,
						t_expand_state *st)
{
	if (expand_add_char(st, str[i]) < 0)
		return (-1);
	return (i + 1);
}
