#include "../minishell.h"

int ft_pwd(t_command *cmd)
{
    char *cwd;
    (void)cmd;
    
    cwd = getcwd(NULL, 0);
    if (!cwd)
    {
        perror("minishell: pwd");
        return (1);
    }
    printf("%s\n", cwd);
    free(cwd);
    return (0);
}