/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_h_02.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:44:31 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/05 14:50:28 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	extract_name_value(const char *arg, char *eq, char **name, char **value)
{
	size_t	name_len;

	name_len = eq - arg;
	*name = ft_substr(arg, 0, name_len);
	*value = ft_strdup(eq + 1);
	if (!(*name) || !(*value))
	{
		free(*name);
		free(*value);
		return (1);
	}
	return (0);
}

void	export_var_eq(t_shell *shell, const char *arg, char *eq)
{
	char	*name;
	char	*value;

	if (extract_name_value(arg, eq, &name, &value) != 0)
		return ;
	if (!is_valid_varname(name))
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		shell->exit_status = 1;
		free(name);
		free(value);
		return ;
	}
	add_or_replace_var(shell, name, value);
	free(name);
	free(value);
}

void	export_var_no_eq(t_shell *shell, const char *arg)
{
	char	*name;

	name = ft_strdup(arg);
	if (!name)
		return ;
	if (!is_valid_varname(name))
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		shell->exit_status = 1;
	}
	free(name);
}

void	export_var(t_shell *shell, const char *arg)
{
	char	*plus_eq;
	char	*eq;

	plus_eq = strstr(arg, "+=");
	if (plus_eq)
	{
		export_var_plus(shell, arg, plus_eq);
		return ;
	}
	eq = ft_strchr(arg, '=');
	if (eq)
	{
		export_var_eq(shell, arg, eq);
		return ;
	}
	else
	{
		export_var_no_eq(shell, arg);
		return ;
	}
}

int	count_env_vars(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->env && shell->env[i])
		i = i + 1;
	return (i);
}
