#include "../minishell.h"

int ft_pwd(t_command *cmd, t_shell *shell)
{
    (void)cmd;

    char *cwd = getcwd(NULL, 0);
    if (!cwd)
    {
        perror("pwd");
        shell->exit_status = 1;
        return 1;
    }

    printf("%s\n", cwd);
    free(cwd);

    shell->exit_status = 0;
    return 0;
}
