#include "../minishell.h"

extern char **environ;

void unset_var(char *var)
{
    if (!is_valid_varname(var))
    {
        perror("unset : is not a valid variable\n");
        return ;
    }
    unsetenv(var);
}

void ft_unset(t_command *cmd)
{
    int i;

    i = 1;
    if (!cmd->args[1])
        return ;
    while (cmd->args[i])
    {
        unset_var(cmd->args[i]);
        i++;
    }

}