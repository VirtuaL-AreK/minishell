/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:15:47 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/06 11:17:15 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	process_escape_char(const char *s, int *i, char quote)
{
	char	res;

	if (s[*i] == 'n')
		res = '\n';
	else if (s[*i] == 't')
		res = '\t';
	else if (s[*i] == '\\')
		res = '\\';
	else if (s[*i] == quote)
		res = quote;
	else
		res = s[*i];
	(*i) = (*i) + 1;
	return (res);
}

void	process_backslash(const char *s, int *i, int *j,
		char *result, char quote)
{
	(*i) = (*i) + 1;
	if (s[*i])
	{
		result[*j] = process_escape_char(s, i, quote);
		(*j) = (*j) + 1;
	}
}

char	*process_ansi_c(const char *s)
{
	int		i;
	int		j;
	int		len;
	char	*result;

	i = 0;
	j = 0;
	len = ft_strlen(s);
	result = malloc(len + 1);
	if (result == NULL)
		return (NULL);
	while (s[i])
	{
		if (s[i] == '\\')
			process_backslash(s, &i, &j, result, '\'');
		else
		{
			result[j] = s[i];
			i = i + 1;
			j = j + 1;
		}
	}
	result[j] = '\0';
	return (result);
}

char	*process_dollar_dquote(const char *s)
{
	int		i;
	int		j;
	int		len;
	char	*result;

	i = 0;
	j = 0;
	len = ft_strlen(s);
	result = malloc(len + 1);
	if (result == NULL)
		return (NULL);
	while (s[i])
	{
		if (s[i] == '\\')
			process_backslash(s, &i, &j, result, '"');
		else
		{
			result[j] = s[i];
			i = i + 1;
			j = j + 1;
		}
	}
	result[j] = '\0';
	return (result);
}

void	append_string(t_expand_state *state, const char *s)
{
	int	k;

	k = 0;
	while (s[k] && state->idx < 4095)
	{
		state->buffer[state->idx] = s[k];
		state->idx = state->idx + 1;
		k = k + 1;
	}
}
