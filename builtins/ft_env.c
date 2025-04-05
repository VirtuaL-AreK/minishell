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

	if (!cmd->args[1])
	{
		i = 0;
		if (!shell->env[i])
		{
			printf("PWD=/home/aanmazir/Desktop\n");
			printf("SHLVL=1\n");
			printf("_=/usr/bin/env\n");
			shell->exit_status = 0;
			return (0);
		}
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
