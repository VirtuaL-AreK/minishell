#include "../minishell.h"

char *ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
    if (!s1 || !s2 || !s3)
        return NULL;
    size_t len1 = ft_strlen(s1);
    size_t len2 = ft_strlen(s2);
    size_t len3 = ft_strlen(s3);
    char *result = malloc(len1 + len2 + len3 + 1);
    if (!result)
        return NULL;
    ft_memcpy(result, s1, len1);
    ft_memcpy(result + len1, s2, len2);
    ft_memcpy(result + len1 + len2, s3, len3);
    result[len1 + len2 + len3] = '\0';
    return result;
}

char *add_or_replace_var(t_shell *shell, const char *name, const char *value)
{
    int i = 0;
    int len = ft_strlen(name);
    char *new_var = ft_strjoin3(name, "=", value);  // "NAME=VALUE"
    if (!new_var)
        return NULL;

    while (shell->env && shell->env[i])
    {
        if (ft_strncmp(shell->env[i], name, len) == 0 && shell->env[i][len] == '=')
        {
            free(shell->env[i]);
            shell->env[i] = new_var;
            return new_var; 
        }
        i++;
    }

    // pas trouvé => on l'ajoute
    char **new_env = malloc(sizeof(char *) * (i + 2));
    if (!new_env)
    {
        free(new_var);
        return NULL;
    }
    int j = 0;
    while (j < i)
    {
        new_env[j] = shell->env[j];
        j++;
    }
    new_env[j] = new_var;
    new_env[j + 1] = NULL;
    free(shell->env);
    shell->env = new_env;
    return new_var;
}

int is_valid_varname(char *var)
{
    int i = 0;

    if (!var || (!ft_isalpha(var[0]) && var[0] != '_'))
        return 0;

    while (var[++i])
    {
        if (!ft_isalnum(var[i]) && var[i] != '_')
            return 0;
    }
    return 1;
}

void export_var(t_shell *shell, const char *arg)
{
    char *eq = ft_strchr(arg, '='); 
    char *name;
    char *value;

    if (!eq)
    {
        name = ft_strdup(arg);
        value = ft_strdup("");
    }
    else
    {
        size_t name_len = eq - arg;
        name = ft_substr(arg, 0, name_len);
        value = ft_strdup(eq + 1);
    }

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

static int compare_env_vars(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

static void print_sorted_env(t_shell *shell)
{
    int i = 0;
    while (shell->env && shell->env[i])
        i++;

    char **sorted_env = malloc(sizeof(char *) * (i + 1));
    if (!sorted_env)
    {
        perror("malloc");
        shell->exit_status = 1;
        return;
    }

    for (int j = 0; j < i; j++)
        sorted_env[j] = shell->env[j];
    sorted_env[i] = NULL;

    qsort(sorted_env, i, sizeof(char *), compare_env_vars);

    for (int j = 0; j < i; j++)
    {
        char *eq = ft_strchr(sorted_env[j], '=');
        if (eq)
        {
            printf("declare -x ");
            fwrite(sorted_env[j], 1, eq - sorted_env[j] + 1, stdout);
            printf("\"%s\"\n", eq + 1);
        }
        else
        {
            printf("declare -x %s\n", sorted_env[j]);
        }
    }

    free(sorted_env);
    shell->exit_status = 0;
}

int ft_export(t_command *cmd, t_shell *shell)
{
    if (!cmd->args[1])
    {
        print_sorted_env(shell);
        return 0;
    }

    int i = 1;
    while (cmd->args[i])
    {
        if (ft_strlen(cmd->args[i]) == 0 || strcmp(cmd->args[i], "=") == 0)
        {
            shell->exit_status = 1;
            ft_putstr_fd("export: not a valid identifier\n", 2);
            i++;
            continue;
        }

        char *eq = ft_strchr(cmd->args[i], '=');
        if (eq)
        {
            size_t name_len = eq - cmd->args[i];
            char *name = ft_substr(cmd->args[i], 0, name_len);
            if (!is_valid_varname(name))
            {
                ft_putstr_fd("export: `", 2);
                ft_putstr_fd(name, 2);
                ft_putstr_fd("': not a valid identifier\n", 2);
                shell->exit_status = 1;
                free(name);
                i++;
                continue;
            }
            free(name);
        }
        else
        {
            if (!is_valid_varname(cmd->args[i]))
            {
                ft_putstr_fd("export: `", 2);
                ft_putstr_fd(cmd->args[i], 2);
                ft_putstr_fd("': not a valid identifier\n", 2);
                shell->exit_status = 1;
                i++;
                continue;
            }
        }

        export_var(shell, cmd->args[i]);
        i++;
    }
    return (shell->exit_status == 1 ? 1 : 0);
}