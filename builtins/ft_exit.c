/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:05:16 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/24 10:34:27 by aanmazir         ###   ########.fr       */
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

static void	exit_numeric_error(const char *arg)
{
	ft_putstr_fd("exit\n", 2);
	ft_putstr_fd("bash: exit: ", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	exit(2);
}

static long long	ft_atol(const char *s)
{
	long long	res;
	int			sign;
	int			i;

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
		if (res > INT_MAX)
			exit_numeric_error(s);
		if (res < INT_MIN)
			exit_numeric_error(s);
		i++;
	}
	return (res * sign);
}

static int	handle_too_many_args(t_command *cmd, t_shell *shell)
{
	if (cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		shell->exit_status = 1;
		return (1);
	}
	return (0);
}

int	ft_exit(t_command *cmd, t_shell *shell)
{
	char	*arg;
	long	val;

	arg = cmd->args[1];
	if (!arg)
	{
		printf("exit\n");
		exit(shell->exit_status);
	}
	if (!is_numeric_argument(arg))
		exit_numeric_error(arg);
	val = ft_atol(arg);
	if (handle_too_many_args(cmd, shell))
		return (1);
	exit((unsigned char)val);
}
