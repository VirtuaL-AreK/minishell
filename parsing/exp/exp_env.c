/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:04:56 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/06 11:13:29 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_local_env_value(const char *var, t_shell *shell)
{
	int		i;
	int		len;
	char	*ret;

	i = 0;
	len = ft_strlen(var);
	while (shell->env && shell->env[i])
	{
		if (ft_strncmp(shell->env[i], var, len) == 0
			&& shell->env[i][len] == '=')
		{
			ret = ft_strdup(shell->env[i] + len + 1);
			return (ret);
		}
		i = i + 1;
	}
	return (ft_strdup(""));
}

void	handle_alphanum_variable(const char *s, int *i,
		t_expand_state *state, t_shell *shell)
{
	int		start;
	int		var_len;
	char	*var_name;
	char	*val;

	start = *i;
	while (s[*i] && (((s[*i] >= 'A' && s[*i] <= 'Z')
				|| (s[*i] >= 'a' && s[*i] <= 'z')
				|| (s[*i] >= '0' && s[*i] <= '9'))
			|| s[*i] == '_'))
		(*i) = (*i) + 1;
	var_len = *i - start;
	var_name = strndup(s + start, var_len);
	val = get_local_env_value(var_name, shell);
	free(var_name);
	append_string(state, val);
	free(val);
}

void	handle_variable(const char *s, int *i,
		t_expand_state *state, t_shell *shell)
{
	char	*exit_str;

	(*i) = (*i) + 1;
	if (s[*i] == '?')
	{
		(*i) = (*i) + 1;
		exit_str = ft_itoa(shell->exit_status);
		append_string(state, exit_str);
		free(exit_str);
	}
	else if ((s[*i] >= 'A' && s[*i] <= 'Z')
		|| (s[*i] >= 'a' && s[*i] <= 'z')
		|| s[*i] == '_')
	{
		handle_alphanum_variable(s, i, state, shell);
	}
	else
	{
		state->buffer[state->idx] = '$';
		state->idx = state->idx + 1;
	}
}

void	handle_dollar_quoted(const char *s, int *i,
		t_expand_state *state, t_shell *shell)
{
	char	quote;
	int		seg_start;
	int		seg_len;
	char	*segment;
	char	*processed;

	(void)shell;
	quote = s[*i + 1];
	*i = *i + 2;
	seg_start = *i;
	while (s[*i] && s[*i] != quote)
		(*i) = (*i) + 1;
	seg_len = *i - seg_start;
	segment = strndup(s + seg_start, seg_len);
	if (quote == '\'')
		processed = process_ansi_c(segment);
	else
		processed = process_dollar_dquote(segment);
	free(segment);
	append_string(state, processed);
	free(processed);
	if (s[*i] == quote)
		(*i) = (*i) + 1;
}
