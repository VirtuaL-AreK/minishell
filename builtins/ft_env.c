/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:01:45 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/05 09:04:51 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_env(t_command *cmd, t_shell *shell)
{
	int	i;

	if (!shell->env[1])
	{
		shell->env[0] = ft_strdup("PWD=/home/aanmazir/Desktop");
		shell->env[1] = ft_strdup("SHLVL=1");
		shell->env[2] = ft_strdup("_=/usr/bin/env");
	}
	if (!cmd->args[1])
	{
		i = 0;
		while (shell->env && shell->env[i])
		{
			printf("%s\n", shell->env[i]);
			i++;
		}
		shell->exit_status = 0;
		return (0);
	}
	shell->exit_status = 0;
	return (0);
}
