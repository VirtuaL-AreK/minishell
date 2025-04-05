/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:11:42 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/05 09:15:20 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(t_command *cmd, t_shell *shell)
{
	char	*cwd;

	(void)cmd;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		shell->exit_status = 1;
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	shell->exit_status = 0;
	return (0);
}
