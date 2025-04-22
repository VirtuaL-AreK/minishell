/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:01:45 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/22 20:19:24 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_env(t_command *cmd, t_shell *shell)
{
	int	i;

	if (!shell->env)
	{
		printf("PWD=/home/aanmazir/Desktop\n");
		printf("SHLVL=1\n");
		printf("_=/usr/bin/env\n");
		shell->exit_status = 0;
		return (0);
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
