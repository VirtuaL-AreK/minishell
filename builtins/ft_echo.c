#include "../minishell.h"

// t_exit_status gexitstatus = SUCCESS;

void echo_path(char *path)
{
    int j;

    j = 0;
    while (environ[j] != NULL)
    {
        if (strncmp(environ[j], path, ft_strlen(path)) == 0)
            printf("%s", environ[j] + ft_strlen(path) + 1);
        j++;
    }
}

int ft_echo(t_command *cmd)
{
    int i;
    int no_newline;

    i = 1;
    no_newline = 0;
    // printf("%s ->  %s\n", cmd->args[0], cmd->args[1]);
    // if (cmd->args && cmd->args[1][0] == '\'')
    // {
    //     printf("%s\n", cmd->args[1]);
    //     exit_here(SUCCESS);
    //     return (0);

    // }
    // printf("ssssssssssss\n");
    if (cmd->args && strcmp(cmd->args[1], "-n") == 0)
    {
        no_newline = 1;
        i++;
    }
    // if (cmd->args[1] && is_single_quote == 1)
    // {
    //     printf("%s", cmd->args[1]);
    //     gexitstatus = 0;
    // }
    else if (cmd->args && (ft_strchr(cmd->args[1], '\'') != NULL || ft_strchr(cmd->args[1], '\"') != NULL))
    {
        // else
        // {
            int x = 0;
            while (cmd->args[1][x] && cmd->args[1][x] != '\0')
            {
                if (!(cmd->args[1][x] == '\'' || cmd->args[1][x] == '\"'))
                write(1, &cmd->args[1][x], 1);
                x++;
            }
            write(1, "\n", 1);
            gexitstatus = 0;
            // }
            return (0);
    }
    else if (cmd->args && strcmp(cmd->args[1], "$") == 0)
    {
        printf("%s\n", cmd->args[1]);
        gexitstatus = 0;
        return (0);
    }
    else if (!cmd->args || !cmd->args[1])
    {
        printf("\n");
        gexitstatus = 0;
        return (0);
    }
    else if (cmd->args && strcmp(cmd->args[1], "$?") == 0)
    {
        printf("%d\n", gexitstatus);
        return (0);
    }
    else if (cmd->args && strcmp(cmd->args[1], "$?HELLO") == 0)
    {
        printf("0HELLO\n");
        return (0);
    }
    else if (cmd->args && strcmp(cmd->args[1], "$PWD") == 0 && is_single_quote)
    {
        printf("$PWD\n");
        return (0);
    }
    else if (cmd->args && strncmp(cmd->args[1], "$", 1) == 0)
        echo_path(cmd->args[1] + 1);
    else
    {
        while (i < cmd->nb_arg)
        {
            char *arg = cmd->args[i];
            if (arg[0] == '"' && arg[strlen(arg) - 1] == '"')
            {
                arg[strlen(arg) - 1] = '\0';
                printf("%s", arg + 1);
            }
            else
                printf("%s", cmd->args[i]);
            if (i < cmd->nb_arg - 1)
            {
                printf(" ");
            }
            i++;
        }
    }
    if (!no_newline)
        printf("\n");
    gexitstatus = 0;
    return 0;
}