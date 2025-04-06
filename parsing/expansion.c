/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 10:56:27 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/06 11:01:50 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	process_dollar_branch(const char *str, int i,
		t_expand_state *state, t_shell *shell)
{
	if (str[i + 1] == '\'' || str[i + 1] == '"')
		handle_dollar_quoted(str, &i, state, shell);
	else
		handle_variable(str, &i, state, shell);
	return (i);
}

static int	process_backslash_branch(const char *str, int i,
		t_expand_state *state)
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

static int	process_char_in_expand(const char *str, int i,
		t_expand_state *state, t_shell *shell)
{
	if (!state->in_sq && str[i] == '$')
		return (process_dollar_branch(str, i, state, shell));
	else if (str[i] == '\'' && !state->in_dq)
	{
		state->in_sq = !state->in_sq;
		return (i + 1);
	}
	else if (str[i] == '"' && !state->in_sq)
	{
		state->in_dq = !state->in_dq;
		return (i + 1);
	}
	else if (str[i] == '\\')
		return (process_backslash_branch(str, i, state));
	else
	{
		state->buffer[state->idx] = str[i];
		state->idx = state->idx + 1;
		return (i + 1);
	}
}

char	*expand_string(const char *str, t_shell *shell)
{
	t_expand_state	state;
	int				i;
	char			*result;

	state.idx = 0;
	state.in_sq = 0;
	state.in_dq = 0;
	i = 0;
	while (str[i] && state.idx < 4095)
	{
		i = process_char_in_expand(str, i, &state, shell);
	}
	state.buffer[state.idx] = '\0';
	result = ft_strdup(state.buffer);
	return (result);
}

void	expand_tokens(t_token *tokens, t_shell *shell)
{
	t_token	*cur;
	char	*expanded;

	cur = tokens;
	while (cur)
	{
		if (cur->type == TOKEN_WORD && cur->should_expand)
		{
			expanded = expand_string(cur->value, shell);
			free(cur->value);
			cur->value = expanded;
		}
		cur = cur->next;
	}
}
