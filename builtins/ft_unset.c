#include "../minishell.h"

void unset_var(char *var)
{
    int i;
    int var_len;
    // int j;

    if (!is_valid_varname(var))
    {
        perror("unset : is not a valid variable\n");
        return ;
    }
    i = 0;
    // j = 0;
    var_len = ft_strlen(var);
    // ft_unsetenv(var);
    while (environ[i] != NULL)
    {
        if ((ft_strncmp(environ[i], var, var_len) == 0) && environ[i][var_len] == '=')
        {
            while (environ[i] != NULL)
            {
                environ[i] = environ [i + 1];
                i++;
            }
            return ;
        }
        i++;
    }
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