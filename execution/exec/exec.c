/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:53:19 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/06 12:01:33 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_free_strarray(char **arr)
{
	int	i;

	i = 0;
	if (arr == NULL)
		return ;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i = i + 1;
	}
	free(arr);
}

char	*get_env_path(char **env)
{
	int		i;
	char	*env_path;

	i = 0;
	env_path = NULL;
	while (env != NULL && env[i] != NULL)
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			env_path = env[i] + 5;
			break ;
		}
		i = i + 1;
	}
	return (env_path);
}

char	*search_in_paths(char *env_path, char *cmd)
{
	char	**paths;
	char	*tmp;
	char	*full_path;
	int		i;

	if (env_path[0] == '\0')
		return (NULL);
	paths = ft_split(env_path, ':');
	if (paths == NULL)
		return (NULL);
	i = 0;
	while (paths[i] != NULL)
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full_path, F_OK) == 0)
			return (ft_free_strarray(paths), full_path);
		free(full_path);
		i = i + 1;
	}
	ft_free_strarray(paths);
	return (NULL);
}

char	*search_in_cwd(char *cmd)
{
	char	*cwd;
	char	*tmp;
	char	*full_path;
	size_t	len;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (NULL);
	len = ft_strlen(cwd);
	if (len >= 4 && ft_strcmp((cwd + len - 4), "/bin") == 0)
	{
		tmp = ft_strjoin(cwd, "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		free(cwd);
		if (access(full_path, F_OK) == 0)
			return (full_path);
		free(full_path);
		return (NULL);
	}
	free(cwd);
	return (NULL);
}

char	*find_exec(char *cmd, char **env)
{
	char	*full_path;
	char	*env_path;

	if (ft_strchr(cmd, '/') != NULL)
	{
		if (access(cmd, F_OK) == 0)
		{
			full_path = ft_strdup(cmd);
			return (full_path);
		}
		else
			return (NULL);
	}
	env_path = get_env_path(env);
	if (env_path != NULL)
	{
		full_path = search_in_paths(env_path, cmd);
		return (full_path);
	}
	full_path = search_in_cwd(cmd);
	return (full_path);
}
