#include "minishell.h"

/**
 * clone_envp:
 *   Duplique le tableau de char** (envp) reçu par main()
 *   pour le stocker dans shell->env.
 */
char **clone_envp(char **envp)
{
    int i = 0;
    while (envp && envp[i])
        i++;

    char **copy = malloc(sizeof(char*) * (i + 1));
    if (!copy)
        return NULL;

    i = 0;
    while (envp && envp[i])
    {
        copy[i] = ft_strdup(envp[i]);
        i++;
    }
    copy[i] = NULL;
    return copy;
}

/**
 * free_envp:
 *   Libère un tableau de char**, comme ft_free_strarray
 */
void free_envp(char **envp)
{
    int i = 0;
    if (!envp)
        return;
    while (envp[i])
    {
        free(envp[i]);
        i++;
    }
    free(envp);
}
