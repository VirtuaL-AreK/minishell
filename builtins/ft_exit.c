/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:05:16 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/22 21:27:53 by iel-kher         ###   ########.fr       */
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

static long	ft_atol(const char *s)
{
	long	res;
	int		sign;
	int		i;

	res = 0;
	sign = 1;
	i = 0;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			sign = -1;
		i++;
	}
	while (s[i] && ft_isdigit(s[i]))
	{
		res = res * 10 + (s[i] - '0');
		i++;
	}
	return (res * sign);
}

int	ft_exit(t_command *cmd, t_shell *shell)
{
	char	*arg;
	long	val;

	// ft_putstr_fd("exit\n", 1);

	arg = cmd->args[1];
	if (!arg)
		exit(shell->exit_status);

	if (!is_numeric_argument(arg))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
	val = ft_atol(arg);
	if (val > LONG_MAX || val < LONG_MIN)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}

	if (cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		shell->exit_status = 1;
		return (1);
	}

	exit((unsigned char)val);
}
