/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 08:47:51 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/19 15:50:18 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_valid_echo_flag(const char *arg)
{
	int	j;
	int	valid;

	j = 1;
	valid = 1;
	while (arg[j])
	{
		if (arg[j] != 'n')
		{
			valid = 0;
			break ;
		}
		j = j + 1;
	}
	return (valid);
}

static int	parse_echo_flags(t_command *cmd, int *start_index)
{
	int	i;
	int	no_newline;
	int	valid;

	i = 1;
	no_newline = 0;
	while (cmd->args[i] && cmd->args[i][0] == '-' && cmd->args[i][1])
	{
		valid = is_valid_echo_flag(cmd->args[i]);
		if (!valid)
			break ;
		no_newline = 1;
		i = i + 1;
	}
	*start_index = i;
	return (no_newline);
}

static void	print_echo_output(t_command *cmd, int start_index)
{
	int	i;

	i = start_index;
	while (i < cmd->nb_arg)
	{
		printf("%s", cmd->args[i]);
		if (i < cmd->nb_arg - 1)
			putchar(' ');
		i++;
	}
}

int	ft_echo(t_command *cmd, t_shell *shell)
{
	int	start_index;
	int	no_newline;

	no_newline = parse_echo_flags(cmd, &start_index);
	print_echo_output(cmd, start_index);
	if (!no_newline)
		putchar('\n');
	shell->exit_status = 0;
	return (0);
}
