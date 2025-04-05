/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:05:16 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/05 09:07:54 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_numeric_argument(const char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(t_command *cmd, t_shell *shell)
{
	int	code;

	if (!cmd->args[1])
		exit(shell->exit_status);
	if (!is_numeric_argument(cmd->args[1]))
	{
		ft_putstr_fd(" numeric argument required\n", 2);
		exit(2);
	}
	if (cmd->args[2])
	{
		ft_putstr_fd(" too many arguments\n", 2);
		shell->exit_status = 1;
		return (1);
	}
	code = ft_atoi(cmd->args[1]);
	exit(code);
}
