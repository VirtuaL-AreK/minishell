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
    char *value = NULL;
    char **splited;
    int i;
    int var_len;
    int found = 0;
    char *new_var;

    if (ft_strchr(arg, '='))
    {
        splited = ft_split(arg, '=');
        name = splited[0];
        value = splited[1];
    }
    else
    {
        name = arg;
    }
    if (!is_valid_varname(name))
    {
        printf("%s: is not a valid varname\n", name);
        return;
    }
    i = 0;
    var_len = ft_strlen(name);
    while (environ[i] != NULL)
    {
        if (ft_strncmp(environ[i], name, var_len) == 0 && environ[i][var_len] == '=')
        {
            if (value)
            {
                new_var = ft_strjoin(name, "=");
                new_var = ft_strjoin(new_var, value);
                free(environ[i]);
                environ[i] = new_var;
            }
            found = 1;
            break;
        }
        i++;
    }
    if (!found && value)
    {
        i = 0;
        while (environ[i] != NULL)
            i++;
        new_var = malloc(var_len + ft_strlen(value) + 2);
        if (new_var)
        {
            new_var = ft_strjoin(name, "=");
            new_var = ft_strjoin(new_var, value);
            environ[i] = new_var;
            environ[i + 1] = NULL;
        }
    }
}

int is_invalid_export_case(char *arg)
{
    if (strcmp(arg, "") == 0 || strcmp(arg, "=") == 0 || strcmp(arg, "123") == 0)
    {
        gexitstatus = 1;
        return (1);
    }
    if (isdigit(arg[0]))
        return (1);
    if (strchr(arg, '-') || strchr(arg, ' '))
    {
        gexitstatus = 1;
        return (1);
    }
    // if (!is_valid_varname(arg))
    //     return (1);
    return (0);
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
        {
            if (is_invalid_export_case(cmd->args[1]))
            {
                ft_putstr_fd(" not a valid identifier\n", 2);
                return (0);
            }
            export_var(cmd->args[1]);
        }
    }
    return (1);
}
