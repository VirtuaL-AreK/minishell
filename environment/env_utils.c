/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:27:11 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/24 09:53:40 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	**create_default_env(void)
{
	char	**copy;

	copy = malloc(sizeof(char *) * 3);
	if (!copy)
		return (NULL);
	copy[0] = ft_strdup("PWD=/home/aanmazir/Desktop");
	copy[1] = ft_strdup("_=/usr/bin/env");
	copy[2] = NULL;
	return (copy);
}

static char	**duplicate_envp(char **envp)
{
	int		i;
	char	**copy;

	i = 0;
	while (envp[i])
		i++;
	copy = malloc(sizeof(char *) * (i + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		copy[i] = ft_strdup(envp[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

char	**clone_envp(char **envp, t_shell *shell)
{
	char	**copy;

	if (!envp || !*envp)
	{
		copy = create_default_env();
		shell->env = copy;
		return (handle_shlvl(shell->env, shell));
	}
	copy = duplicate_envp(envp);
	if (!copy)
		return (NULL);
	shell->env = copy;
	return (handle_shlvl(shell->env, shell));
}

void	free_envp(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}
