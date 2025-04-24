/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_h_01.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 13:49:07 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/22 22:09:47 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_valid_varname(char *var)
{
	int	i;

	i = 0;
	if (!var || (!ft_isalpha(var[0]) && var[0] != '_'))
		return (0);
	while (var[i + 1])
	{
		i = i + 1;
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (0);
	}
	return (1);
}

int	export_var_plus_get_parts(const char *arg, char *plus_eq,
	t_export_parts *parts, t_shell *shell)
{
	size_t	name_len;

	name_len = plus_eq - arg;
	parts->name = ft_substr(arg, 0, name_len);
	parts->to_append = ft_strdup(plus_eq + 2);
	if (!parts->name || !parts->to_append)
	{
		free(parts->name);
		free(parts->to_append);
		return (1);
	}
	if (!is_valid_varname(parts->name))
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(parts->name, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		shell->exit_status = 1;
		free(parts->name);
		free(parts->to_append);
		return (1);
	}
	return (0);
}

char	*export_var_plus_get_old_val(t_shell *shell, const char *name)
{
	int		i;
	int		found;
	char	*old_val;

	i = 0;
	found = 0;
	old_val = NULL;
	while (shell->env && shell->env[i])
	{
		if (ft_strncmp(shell->env[i], name, ft_strlen(name)) == 0
			&& shell->env[i][ft_strlen(name)] == '=')
		{
			found = 1;
			old_val = ft_strdup(shell->env[i] + ft_strlen(name) + 1);
			break ;
		}
		i = i + 1;
	}
	if (!found)
		old_val = ft_strdup("");
	return (old_val);
}

void	export_var_plus_update(t_shell *shell, const char *name,
	const char *to_append, char *old_val)
{
	char	*new_val;

	new_val = ft_strjoin(old_val, to_append);
	add_or_replace_var(shell, name, new_val);
	free(new_val);
}

void	export_var_plus(t_shell *shell, const char *arg, char *plus_eq)
{
	t_export_parts	parts;
	char			*old_val;

	if (export_var_plus_get_parts(arg, plus_eq, &parts, shell) != 0)
		return ;
	old_val = export_var_plus_get_old_val(shell, parts.name);
	export_var_plus_update(shell, parts.name, parts.to_append, old_val);
	free(old_val);
	free(parts.to_append);
	free(parts.name);
}
