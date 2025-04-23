/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:27:11 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/23 16:55:43 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int find_shlvl_index(char **env)
{
    int i = 0;
    while (env && env[i])
    {
        if (ft_strncmp(env[i], "SHLVL=", 6) == 0)
            return (i);
        i++;
    }
    return (-1);
}

static char *format_shlvl(int lvl)
{
    char *num = ft_itoa(lvl);
    char *res;

    if (!num)
        return (NULL);
    res = ft_strjoin("SHLVL=", num);
    free(num);
    return (res);
}

char **handle_shlvl(char **env, t_shell *shell)
{
    int   idx = find_shlvl_index(env);
    char  *newvar;

    if (idx >= 0)
    {
        int   oldlvl = ft_atoi(env[idx] + 6) + 1;
        newvar = format_shlvl(oldlvl);
        if (newvar)
        {
            free(env[idx]);
            env[idx] = newvar;
        }
    }
    else
    {
        newvar = format_shlvl(1);
        if (newvar)
        {
            export_var(shell, newvar);
            free(newvar);
        }
    }
    return (shell->env);
}


static char **create_default_env(void)
{
    char **copy = malloc(sizeof(char *) * 2);
    if (!copy)
        return (NULL);
    copy[0] = ft_strdup("PATH=/usr/local/bin:/usr/bin:/bin");
    copy[1] = NULL;
    return (copy);
}

static char **duplicate_envp(char **envp)
{
    int    i = 0;
    char **copy;

    while (envp[i])
        i++;
    copy = malloc(sizeof(char *) * (i + 1));
    if (!copy)
        return (NULL);
    i = 0;
    while (envp[i])
    {
        copy[i] = ft_strdup(envp[i]);
        i++;
    }
    copy[i] = NULL;
    return (copy);
}

char **clone_envp(char **envp, t_shell *shell)
{
    char **copy;

    if (!envp || !*envp)
    {
        copy = create_default_env();
        shell->env = copy;
        return (handle_shlvl(shell->env, shell));
    }
    copy = duplicate_envp(envp);
    if (!copy)
        return (NULL);
    shell->env = copy;
    if (get_env_path(shell->env) == NULL)
        add_or_replace_var(shell, "PATH", "/usr/local/bin:/usr/bin:/bin");
    return (handle_shlvl(shell->env, shell));
}

void	free_envp(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}
