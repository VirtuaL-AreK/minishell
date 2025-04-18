/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:27:11 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/18 11:38:39 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **handle_shlvl(char **envir, t_shell *shell)
{
    int i;
    char *var;
    int n;
    char *new_value;
    char *sm;

    i = 0;
    while (envir[i])
    {
        if (strncmp(envir[i], "SHLVL=", 6) == 0)
        {
            var = strdup(envir[i] + 6);
            if (var == NULL)
            {
                ft_putstr_fd("strdup failed", 2);
                return (envir);
            }
            n = ft_atoi(var);
            free(var);
            n++;
            new_value = malloc(8 * sizeof(char));
            if (new_value == NULL)
            {
                ft_putstr_fd("malloc failed", 2);
                return (envir);
            }
            ft_strlcpy(new_value, "SHLVL=", 7);
            sm = ft_itoa(n);
            var = ft_strjoin(new_value, sm);
            free(envir[i]);
            envir[i] = var;
            free(new_value);
            free(sm);
            return (envir);
        }
        i++;
    }
    var = ft_strjoin("SHLVL=", "1");
    if (var == NULL)
    {
        ft_putstr_fd("ft_strjoin failed", 2);
        return (envir);
    }
    export_var(shell, var);
    free(var);
    return (envir);
}

char **clone_envp(char **envp, t_shell *shell)
{
    int   i;
    char **copy;

    i = 0;
    while (envp && envp[i])
        i++;
    copy = malloc(sizeof(char *) * (i + 1));
    if (!copy)
        return (NULL);
    i = 0;
    while (envp && envp[i])
    {
        copy[i] = ft_strdup(envp[i]);
        i++;
    }
    copy[i] = NULL;
    return handle_shlvl(copy, shell);
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
