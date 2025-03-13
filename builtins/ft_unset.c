#include "../minishell.h"

static void unset_var_from_env(t_shell *shell, const char *name)
{
    int i = 0;
    int len = ft_strlen(name);

    while (shell->env && shell->env[i])
    {
        if (ft_strncmp(shell->env[i], name, len) == 0 && shell->env[i][len] == '=')
        {
            free(shell->env[i]);
            while (shell->env[i + 1])
            {
                shell->env[i] = shell->env[i + 1];
                i++;
            }
            shell->env[i] = NULL;
            return;
        }
        i++;
    }
}

int ft_unset(t_command *cmd, t_shell *shell)
{
    int i = 1;
    while (cmd->args[i])
    {
        unset_var_from_env(shell, cmd->args[i]);
        i++;
    }
    shell->exit_status = 0;
    return 0;
}
