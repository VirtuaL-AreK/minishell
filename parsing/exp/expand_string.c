/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:02:02 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/24 15:05:22 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	process_char_in_expand(const char *str,
							int i,
							t_expand_state *st,
							t_shell *shell)
{
	int	ret;

	if (!st->in_sq && str[i] == '$')
		return (handle_dollar_expand(str, &i, st, shell));
	ret = handle_quote_toggle(str, i, st);
	if (ret >= 0)
		return (ret);
	if (str[i] == '\\')
		return (handle_backslash_expand(str, i, st));
	return (handle_regular_char(str, i, st));
}

char	*expand_string(const char *str, t_shell *shell)
{
	t_expand_state	st;
	int				i;
	int				ret;

	(void)shell;
	if (!str)
		return (NULL);
	if (init_expand_state(&st, 64) < 0)
		return (NULL);
	i = 0;
	while (str[i])
	{
		ret = process_char_in_expand(str, i, &st, shell);
		if (ret < 0)
		{
			free(st.buffer);
			return (NULL);
		}
		i = ret;
	}
	if (expand_add_char(&st, '\0') < 0)
	{
		return (free(st.buffer), NULL);
	}
	return (st.buffer);
}

char	*expand_tilde(const char *value, t_shell *shell)
{
	char	*home;
	char	*result;

	if (!value || value[0] != '~')
		return (ft_strdup(value));
	if (value[1] == '\0' || value[1] == '/')
	{
		home = get_local_env_value("HOME", shell);
		if (!home)
			return (ft_strdup(value));
		if (value[1] == '/')
		{
			result = ft_strjoin(home, value + 1);
		}
		else
		{
			result = home;
		}
		return (result);
	}
	return (ft_strdup(value));
}

char	*remove_quotes(const char *s)
{
	t_expand_state	st;
	int				in_sq;
	int				in_dq;
	int				i;

	in_sq = 0;
	in_dq = 0;
	i = 0;
	if (init_expand_state(&st, 64) < 0)
		return (NULL);
	while (s[i])
	{
		if (s[i] == '\'' && !in_dq)
			in_sq = !in_sq;
		else if (s[i] == '"' && !in_sq)
			in_dq = !in_dq;
		else
			expand_add_char(&st, s[i]);
		i++;
	}
	expand_add_char(&st, '\0');
	return (st.buffer);
}
