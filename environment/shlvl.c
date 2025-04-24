/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 09:58:05 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/24 09:58:09 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_shlvl_index(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		if (ft_strncmp(env[i], "SHLVL=", 6) == 0)
			return (i);
		i++;
	}
	return (-1);
}

char	*format_shlvl(int lvl)
{
	char	*num;
	char	*res;

	num = ft_itoa(lvl);
	if (!num)
		return (NULL);
	res = ft_strjoin("SHLVL=", num);
	free(num);
	return (res);
}

char	**handle_shlvl(char **env, t_shell *shell)
{
	int			idx;
	int			oldlvl;
	char		*newvar;

	idx = find_shlvl_index(env);
	if (idx >= 0)
	{
		oldlvl = ft_atoi(env[idx] + 6) + 1;
		newvar = format_shlvl(oldlvl);
		if (newvar)
		{
			free(env[idx]);
			env[idx] = newvar;
		}
	}
	else
	{
		newvar = format_shlvl(1);
		if (newvar)
		{
			export_var(shell, newvar);
			free(newvar);
		}
	}
	return (shell->env);
}
