/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 20:28:14 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/24 13:38:00 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_strjoin_and_free(char *s1, const char *s2)
{
	char	*new_str;
	size_t	len1;
	size_t	len2;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (s1);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	new_str = malloc(len1 + len2 + 1);
	if (!new_str)
		return (free(s1), NULL);
	ft_memcpy(new_str, s1, len1);
	ft_memcpy(new_str + len1, s2, len2);
	new_str[len1 + len2] = '\0';
	free(s1);
	return (new_str);
}

void	append_char_result(char c, char **result)
{
	char	temp[2];

	temp[0] = c;
	temp[1] = '\0';
	*result = ft_strjoin_and_free(*result, temp);
}

int	expand_variable_name(const char *line, int i, char **result, t_shell *shell)
{
	int		start_var;
	int		var_len;
	char	*var_name;
	char	*env_val;

	start_var = i;
	while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
		i++;
	var_len = i - start_var;
	if (var_len > 0)
	{
		var_name = ft_substr(line, start_var, var_len);
		env_val = get_local_env_value(var_name, shell);
		*result = ft_strjoin_and_free(*result, env_val);
		free(var_name);
		free(env_val);
	}
	else
		*result = ft_strjoin_and_free(*result, "$");
	return (i);
}

void	expand_exit_status(char **result, t_shell *shell)
{
	char	*exit_str;

	exit_str = ft_itoa(shell->exit_status);
	*result = ft_strjoin_and_free(*result, exit_str);
	free(exit_str);
}

int	process_dollar_in_heredoc(const char *line, int i, char **result,
		t_shell *shell)
{
	i++;
	if (line[i] == '?')
	{
		i++;
		expand_exit_status(result, shell);
	}
	else
		i = expand_variable_name(line, i, result, shell);
	return (i);
}
