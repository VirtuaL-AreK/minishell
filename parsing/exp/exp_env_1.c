/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_env_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:33:49 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/24 17:34:46 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	handle_exit_status(t_shell *shell, t_expand_state *st)
{
	char	*exit_str;

	exit_str = ft_itoa(shell->exit_status);
	if (!exit_str)
		return (-1);
	if (append_string(st, exit_str) < 0)
	{
		free(exit_str);
		return (-1);
	}
	free(exit_str);
	return (0);
}

int	handle_alphanumeric_variable(const char *s, int *i,
		t_expand_state *st, t_shell *shell)
{
	return (handle_alphanum_variable(s, i, st, shell));
}

int	handle_default_variable(t_expand_state *st)
{
	return (expand_add_char(st, '$'));
}
