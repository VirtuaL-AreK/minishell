#include "minishell.h"

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
