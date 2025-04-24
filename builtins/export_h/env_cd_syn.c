/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cd_syn.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:15:40 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/24 10:17:56 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*resolve_cd_double_dash(t_command *cmd, t_shell *shell)
{
	if (cmd->nb_arg == 2)
		return (resolve_cd_path_tilde(cmd, shell));
	if (cmd->nb_arg == 3)
		return (cmd->args[2]);
	ft_putstr_fd("cd: too many arguments\n", 2);
	shell->exit_status = 1;
	return (NULL);
}

char	*resolve_cd_path(t_command *cmd, t_shell *shell)
{
	char	*arg1;

	if (cmd->nb_arg == 1)
		return (resolve_cd_path_tilde(cmd, shell));
	arg1 = cmd->args[1];
	if (ft_strncmp(arg1, "--", 2) == 0)
		return (resolve_cd_double_dash(cmd, shell));
	if (ft_strncmp(arg1, "-", 1) == 0 && cmd->nb_arg == 2)
		return (resolve_cd_path_dash(cmd, shell));
	if (ft_strncmp(arg1, "~/", 2) == 0)
		return (resolve_cd_path_home_slash(cmd, shell));
	if (cmd->nb_arg > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		shell->exit_status = 1;
		return (NULL);
	}
	return (arg1);
}
