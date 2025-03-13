#include "../minishell.h"

int ft_env(t_command *cmd, t_shell *shell)
{
    if (!cmd->args[1])
    {
        int i = 0;
        while (shell->env && shell->env[i])
        {
            printf("%s\n", shell->env[i]);
            i++;
        }
        shell->exit_status = 0;
        return 0;
    }
    shell->exit_status = 0;
    return 0;
}
