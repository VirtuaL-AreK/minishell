/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:27:11 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/22 20:11:19 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **handle_shlvl(char **envir, t_shell *shell)
{
    int     i;
    char    *var;
    int      n;
    char    *sm;
    char    *new_value;

    i = 0;
    while (envir[i])
    {
        if (ft_strncmp(envir[i], "SHLVL=", 6) == 0)
        {
            var = ft_strdup(envir[i] + 6);
            if (!var)
                return (shell->env);
            n = ft_atoi(var) + 1;
            free(var);
            sm = ft_itoa(n);
            if (!sm)
                return (shell->env);
            new_value = ft_strjoin("SHLVL=", sm);
            free(sm);
            if (!new_value)
                return (shell->env);
            free(envir[i]);
            envir[i] = new_value;
            return (shell->env);
        }
        i++;
    }
    var = ft_strjoin("SHLVL=", "1");
    if (var)
    {
        export_var(shell, var);
        free(var);
    }
    return (shell->env);
}

char **clone_envp(char **envp, t_shell *shell)
{
    char **copy;
    int   i;

    if (!envp || !*envp)
    {
        copy = malloc(sizeof(char *) * 2);
        if (!copy)
            return (NULL);
        copy[0] = ft_strdup("PATH=/usr/local/bin:/usr/bin:/bin");
        copy[1] = NULL;
        shell->env = copy;
        return (handle_shlvl(shell->env, shell));
    }

    i = 0;
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
