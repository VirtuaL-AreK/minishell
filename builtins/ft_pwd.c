/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:11:42 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/09 19:25:38 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

int	ft_pwd(t_command *cmd, t_shell *shell)
{
	char	*cwd;

	(void)cmd;
	cwd = getcwd(NULL, 0);
	// if (!cwd)
	// {
	// 	ft_putstr_fd("pwd", 2);
	// 	shell->exit_status = 1;
	// 	return (1);
	// }
	if (!cwd)
	{
		printf("%s\n", shell->path);
		shell->exit_status = 1;
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	shell->exit_status = 0;
	return (0);
}
