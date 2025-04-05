/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_h_00.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 13:35:29 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/05 13:44:36 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	size_t	len1;
	size_t	len2;
	size_t	len3;
	char	*result;

	if (!s1 || !s2 || !s3)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	result = malloc(len1 + len2 + len3 + 1);
	if (!result)
		return (NULL);
	ft_memcpy(result, s1, len1);
	ft_memcpy(result + len1, s2, len2);
	ft_memcpy(result + len1 + len2, s3, len3);
	result[len1 + len2 + len3] = '\0';
	return (result);
}

int	find_var_index(t_shell *shell, const char *name, int len)
{
	int	i;

	i = 0;
	while (shell->env && shell->env[i])
	{
		if (ft_strncmp(shell->env[i], name, len) == 0
			&& shell->env[i][len] == '=')
			return (i);
		i = i + 1;
	}
	return (-1);
}

int	count_env(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->env && shell->env[i])
		i = i + 1;
	return (i);
}

char	*append_var(t_shell *shell, char *new_var, int count)
{
	char	**new_env;
	int		j;

	j = 0;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
	{
		free(new_var);
		return (NULL);
	}
	while (j < count)
	{
		new_env[j] = shell->env[j];
		j = j + 1;
	}
	new_env[j] = new_var;
	new_env[j + 1] = NULL;
	free(shell->env);
	shell->env = new_env;
	return (new_var);
}

char	*add_or_replace_var(t_shell *shell, const char *name, const char *value)
{
	int		len;
	char	*new_var;
	int		index;
	int		count;

	len = ft_strlen(name);
	new_var = ft_strjoin3(name, "=", value);
	if (!new_var)
		return (NULL);
	index = find_var_index(shell, name, len);
	if (index >= 0)
	{
		free(shell->env[index]);
		shell->env[index] = new_var;
		return (new_var);
	}
	count = count_env(shell);
	return (append_var(shell, new_var, count));
}
