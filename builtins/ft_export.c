/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 17:41:56 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/05 17:50:45 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	**copy_env_vars(t_shell *shell, int count)
{
	char	**copy;
	int		j;

	j = 0;
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
	{
		perror("malloc");
		shell->exit_status = 1;
		return (NULL);
	}
	while (j < count)
	{
		copy[j] = shell->env[j];
		j = j + 1;
	}
	copy[count] = NULL;
	return (copy);
}

static void	sort_env_array(char **env_array, int count)
{
	int		j;
	int		swapped;
	char	*temp;

	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		j = 0;
		while (j < count - 1)
		{
			if (strcmp(env_array[j], env_array[j + 1]) > 0)
			{
				temp = env_array[j];
				env_array[j] = env_array[j + 1];
				env_array[j + 1] = temp;
				swapped = 1;
			}
			j = j + 1;
		}
	}
}

static void	print_env_array(char **env_array, int count)
{
	int		j;
	char	*eq;

	j = 0;
	while (j < count)
	{
		eq = ft_strchr(env_array[j], '=');
		if (eq)
		{
			printf("declare -x ");
			fwrite(env_array[j], 1, (size_t)(eq - env_array[j] + 1), stdout);
			printf("\"%s\"\n", eq + 1);
		}
		else
		{
			printf("declare -x %s\n", env_array[j]);
		}
		j = j + 1;
	}
}

static void	print_sorted_env(t_shell *shell)
{
	int		count;
	char	**sorted_env;

	count = count_env_vars(shell);
	sorted_env = copy_env_vars(shell, count);
	if (!sorted_env)
		return ;
	sort_env_array(sorted_env, count);
	print_env_array(sorted_env, count);
	free(sorted_env);
	shell->exit_status = 0;
}

int	ft_export(t_command *cmd, t_shell *shell)
{
	int	i;

	i = 1;
	if (!cmd->args[1])
	{
		print_sorted_env(shell);
		return (0);
	}
	while (cmd->args[i])
	{
		if (ft_strlen(cmd->args[i]) == 0 || strcmp(cmd->args[i], "=") == 0)
		{
			shell->exit_status = 1;
			ft_putstr_fd("export: not a valid identifier\n", 2);
			i = i + 1;
			continue ;
		}
		export_var(shell, cmd->args[i]);
		i = i + 1;
	}
	return (shell->exit_status == 1 ? 1 : 0);
}
