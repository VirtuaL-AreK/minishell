#include "../minishell.h"

static int extract_name_value(const char *arg, char *eq, char **name, char **value)
{
    size_t name_len;

    name_len = eq - arg;
    *name = ft_substr(arg, 0, name_len);
    *value = ft_strdup(eq + 1);
    if (!(*name) || !(*value))
    {
        free(*name);
        free(*value);
        return 1;
    }
    return 0;
}

static void export_var_eq(t_shell *shell, const char *arg, char *eq)
{
    char *name;
    char *value;

    if (extract_name_value(arg, eq, &name, &value) != 0)
        return;
    if (!is_valid_varname(name))
    {
        ft_putstr_fd("export: `", 2);
        ft_putstr_fd(name, 2);
        ft_putstr_fd("': not a valid identifier\n", 2);
        shell->exit_status = 1;
        free(name);
        free(value);
        return;
    }

    add_or_replace_var(shell, name, value);
    free(name);
    free(value);
}

static void export_var_no_eq(t_shell *shell, const char *arg)
{
    char *name = ft_strdup(arg);
    if (!name)
        return;
    if (!is_valid_varname(name))
    {
        ft_putstr_fd("export: `", 2);
        ft_putstr_fd(name, 2);
        ft_putstr_fd("': not a valid identifier\n", 2);
        shell->exit_status = 1;
    }
    free(name);
}

void export_var(t_shell *shell, const char *arg)
{
    char *plus_eq;
    char *eq;

    plus_eq = strstr(arg, "+=");
    if (plus_eq)
    {
        export_var_plus(shell, arg, plus_eq);
        return;
    }
    eq = ft_strchr(arg, '=');
    if (eq)
    {
        export_var_eq(shell, arg, eq);
        return;
    }
    else
    {
        export_var_no_eq(shell, arg);
        return;
    }
}

static int count_env_vars(t_shell *shell)
{
    int i = 0;
    while (shell->env && shell->env[i])
        i = i + 1;
    return i;
}

static char **copy_env_vars(t_shell *shell, int count)
{
    char **copy;
    int j = 0;

    copy = malloc(sizeof(char *) * (count + 1));
    if (!copy)
    {
        perror("malloc");
        shell->exit_status = 1;
        return NULL;
    }
    while (j < count)
    {
        copy[j] = shell->env[j];
        j = j + 1;
    }
    copy[count] = NULL;
    return copy;
}

static void sort_env_array(char **env_array, int count)
{
    int j;
    int swapped;
    char *temp;

    swapped = 1;
    while (swapped)
    {
        swapped = 0;
        j = 0;
        while (j < count - 1)
        {
            if (strcmp(env_array[j], env_array[j + 1]) > 0)
            {
                temp = env_array[j];
                env_array[j] = env_array[j + 1];
                env_array[j + 1] = temp;
                swapped = 1;
            }
            j = j + 1;
        }
    }
}

static void print_env_array(char **env_array, int count)
{
    int j = 0;
    while (j < count)
    {
        char *eq = ft_strchr(env_array[j], '=');
        if (eq)
        {
            printf("declare -x ");
            fwrite(env_array[j], 1, (size_t)(eq - env_array[j] + 1), stdout);
            printf("\"%s\"\n", eq + 1);
        }
        else
        {
            printf("declare -x %s\n", env_array[j]);
        }
        j = j + 1;
    }
}

static void print_sorted_env(t_shell *shell)
{
    int count;
    char **sorted_env;

    count = count_env_vars(shell);
    sorted_env = copy_env_vars(shell, count);
    if (!sorted_env)
        return;
    sort_env_array(sorted_env, count);
    print_env_array(sorted_env, count);
    free(sorted_env);
    shell->exit_status = 0;
}

int ft_export(t_command *cmd, t_shell *shell)
{
    int i = 1;

    if (!cmd->args[1])
    {
        print_sorted_env(shell);
        return 0;
    }
    while (cmd->args[i])
    {
        if (ft_strlen(cmd->args[i]) == 0 || strcmp(cmd->args[i], "=") == 0)
        {
            shell->exit_status = 1;
            ft_putstr_fd("export: not a valid identifier\n", 2);
            i = i + 1;
            continue;
        }
        export_var(shell, cmd->args[i]);
        i = i + 1;
    }

    return (shell->exit_status == 1 ? 1 : 0);
}
