#include "../minishell.h"

int is_valid_varname(char *var)
{
    int i;

    i = 1;
    if (!var || (!ft_isalpha(var[0]) && var[0] != '_'))
        return (0);
    while (var[i])
    {
        if(!ft_isalnum(var[i]) && var[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

void export_var(char *arg)
{
    char *name;
    char **splited;
    char *value;

    if (ft_strchr(arg, '='))
    {
        splited = ft_split(arg, '=');
        name = splited[0];
        value = splited[1];
    }
    else
        name = arg;
    if (!is_valid_varname(name))
    {
        printf("%s: is not a valid varname\n", name);
        return ;
    }
    if (value)
        setenv(name, value, 1);
}

int ft_export(t_command *cmd)
{
    int i;

    if (strcmp(cmd->args[0], "export") == 0)
    {
        if (!cmd->args[1])
        {
            i = 0;
            while (environ[i] != NULL)
            {
                printf("%s\n", environ[i]);
                i++;
            }
        }
        else if (cmd->args[1])
            export_var(cmd->args[1]);
    }
    return (1);
}