#include "../minishell.h"

char *ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
    size_t len1, len2, len3;
    char *result;

    if (!s1 || !s2 || !s3)
        return NULL;
    len1 = ft_strlen(s1);
    len2 = ft_strlen(s2);
    len3 = ft_strlen(s3);
    result = malloc(len1 + len2 + len3 + 1);
    if (!result)
        return NULL;
    ft_memcpy(result, s1, len1);
    ft_memcpy(result + len1, s2, len2);
    ft_memcpy(result + len1 + len2, s3, len3);
    result[len1 + len2 + len3] = '\0';
    return result;
}

static int find_var_index(t_shell *shell, const char *name, int len)
{
    int i = 0;
    while (shell->env && shell->env[i])
    {
        if (ft_strncmp(shell->env[i], name, len) == 0 &&
            shell->env[i][len] == '=')
            return i;
        i = i + 1;
    }
    return -1;
}

static int count_env(t_shell *shell)
{
    int i = 0;
    while (shell->env && shell->env[i])
        i = i + 1;
    return i;
}

static char *append_var(t_shell *shell, char *new_var, int count)
{
    char **new_env;
    int j = 0;

    new_env = malloc(sizeof(char *) * (count + 2));
    if (!new_env)
    {
        free(new_var);
        return NULL;
    }
    while (j < count)
    {
        new_env[j] = shell->env[j];
        j = j + 1;
    }
    new_env[j] = new_var;
    new_env[j + 1] = NULL;
    free(shell->env);
    shell->env = new_env;
    return new_var;
}

char *add_or_replace_var(t_shell *shell, const char *name, const char *value)
{
    int len;
    char *new_var;
    int index, count;

    len = ft_strlen(name);
    new_var = ft_strjoin3(name, "=", value);
    if (!new_var)
        return NULL;
    index = find_var_index(shell, name, len);
    if (index >= 0)
    {
        free(shell->env[index]);
        shell->env[index] = new_var;
        return new_var;
    }
    count = count_env(shell);
    return append_var(shell, new_var, count);
}

int is_valid_varname(char *var)
{
    int i = 0;
    if (!var || (!ft_isalpha(var[0]) && var[0] != '_'))
        return 0;
    while (var[i + 1])
    {
        i = i + 1;
        if (!ft_isalnum(var[i]) && var[i] != '_')
            return 0;
    }
    return 1;
}

typedef struct s_export_parts {
    char *name;
    char *to_append;
} t_export_parts;

static int export_var_plus_get_parts(const char *arg, char *plus_eq,
                                     t_export_parts *parts, t_shell *shell)
{
    size_t name_len;

    name_len = plus_eq - arg;
    parts->name = ft_substr(arg, 0, name_len);
    parts->to_append = ft_strdup(plus_eq + 2);
    if (!parts->name || !parts->to_append)
    {
        free(parts->name);
        free(parts->to_append);
        return 1;
    }
    if (!is_valid_varname(parts->name))
    {
        ft_putstr_fd("export: `", 2);
        ft_putstr_fd(parts->name, 2);
        ft_putstr_fd("': not a valid identifier\n", 2);
        shell->exit_status = 1;
        free(parts->name);
        free(parts->to_append);
        return 1;
    }
    return 0;
}

static char *export_var_plus_get_old_val(t_shell *shell, const char *name)
{
    int i = 0;
    int found = 0;
    char *old_val = NULL;
    while (shell->env && shell->env[i])
    {
        if (ft_strncmp(shell->env[i], name, ft_strlen(name)) == 0 &&
            shell->env[i][ft_strlen(name)] == '=')
        {
            found = 1;
            old_val = ft_strdup(shell->env[i] + ft_strlen(name) + 1);
            break;
        }
        i = i + 1;
    }
    if (!found)
        old_val = ft_strdup("");
    return old_val;
}

static void export_var_plus_update(t_shell *shell, const char *name,
                                   const char *to_append, char *old_val)
{
    char *new_val;

    new_val = ft_strjoin(old_val, to_append);
    add_or_replace_var(shell, name, new_val);
    free(new_val);
}

static void export_var_plus(t_shell *shell, const char *arg, char *plus_eq)
{
    t_export_parts parts;
    char *old_val;

    if (export_var_plus_get_parts(arg, plus_eq, &parts, shell) != 0)
        return;
    old_val = export_var_plus_get_old_val(shell, parts.name);
    export_var_plus_update(shell, parts.name, parts.to_append, old_val);
    free(old_val);
    free(parts.to_append);
    free(parts.name);
}

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
