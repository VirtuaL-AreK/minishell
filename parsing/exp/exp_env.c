/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:04:56 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/23 15:11:32 by iel-kher         ###   ########.fr       */
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

int	handle_alphanum_variable(const char *s, int *i, t_expand_state *st,
		t_shell *shell)
{
	int		start;
	int		var_len;
	char	*var_name;
	char	*val;
	int		ret;

	start = *i;
	while (s[*i] && (((s[*i] >= 'A' && s[*i] <= 'Z') || (s[*i] >= 'a'
					&& s[*i] <= 'z') || (s[*i] >= '0' && s[*i] <= '9'))
			|| s[*i] == '_'))
		(*i)++;
	var_len = *i - start;
	var_name = strndup(s + start, var_len);
	if (!var_name)
		return (-1);
	val = get_local_env_value(var_name, shell);
	free(var_name);
	if (!val)
		return (-1);
	ret = append_string(st, val);
	free(val);
	return (ret);
}

int	handle_variable(const char *s, int *i, t_expand_state *st, t_shell *shell)
{
	char	*exit_str;

	(*i)++;
	if (s[*i] == '?')
	{
		(*i)++;
		exit_str = ft_itoa(shell->exit_status);
		if (!exit_str)
			return (-1);
		if (append_string(st, exit_str) < 0)
		{
			free(exit_str);
			return (-1);
		}
		free(exit_str);
	}
	else if ((s[*i] >= 'A' && s[*i] <= 'Z') || (s[*i] >= 'a' && s[*i] <= 'z')
			|| s[*i] == '_')
	{
		if (handle_alphanum_variable(s, i, st, shell) < 0)
			return (-1);
	}
	else
	{
		if (expand_add_char(st, '$') < 0)
			return (-1);
	}
	return (0);
}

static char	*extract_quoted_segment(const char *s, int *i, char *quote_out)
{
	int		start;
	int		len;
	char	*seg;

	*quote_out = s[*i + 1];
	*i += 2;
	start = *i;
	while (s[*i] && s[*i] != *quote_out)
		(*i)++;
	len = *i - start;
	seg = strndup(s + start, len);
	if (s[*i] == *quote_out)
		(*i)++;
	return (seg);
}

int	handle_dollar_quoted(const char *s, int *i, t_expand_state *st,
		t_shell *shell)
{
	char	quote;
	char	*segment;
	char	*processed;
	int		ret;

	(void)shell;
	segment = extract_quoted_segment(s, i, &quote);
	if (!segment)
		return (-1);
	if (quote == '\'')
		processed = process_ansi_c(segment);
	else
		processed = process_dollar_dquote(segment);
	free(segment);
	if (!processed)
		return (-1);
	ret = append_string(st, processed);
	free(processed);
	if (ret < 0)
		return (-1);
	return (0);
}
