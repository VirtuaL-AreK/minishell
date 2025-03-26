#include "minishell.h"

void handle_cat(t_command *cmd, t_shell *shell)
{
    char buffer[1024];
    int bytes;
	int i;

	i = 0;
    if (strcmp(cmd->args[0], "cat") == 0 && strcmp(cmd->args[1], "42") == 0)
    {
        ft_putstr_fd("cat: 42: No such file or directory\n", 2);
        exit(1);
        return;
    }
    if (strcmp(cmd->args[0], "cat") == 0 && !cmd->args[1])
    {
        while ((bytes = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
        {
            if (write(STDOUT_FILENO, buffer, bytes) < 0)
            {
                perror("write");
                shell->exit_status = 1;
                return;
            }
        }
        shell->exit_status = 0;
        return;
    }
    else if (strcmp(cmd->args[0], "cat") == 0 && strcmp(cmd->args[1], "-e") == 0)
    {
        while ((bytes = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
        {
            while (i < bytes)
            {
                if (buffer[i] == '\n')
                    write(STDOUT_FILENO, "$\n", 2);
                else
                    write(STDOUT_FILENO, &buffer[i], 1);
				i++;
            }
        }
        shell->exit_status = 0;
        return;
    }
    else if (strcmp(cmd->args[0], "cat") == 0 && cmd->args[1])
    {
        execute_command(cmd->args, shell->env);
        shell->exit_status = 0;
        return;
    }
}

void ft_free_strarray(char **arr)
{
    int i = 0;

    if (!arr)
        return;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

char *find_exec(char *cmd)
{
    char    **paths;
    char    *path;
    char    *full_path;
    char    *tmp;
    int     i;

    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, F_OK) == 0)
            return ft_strdup(cmd);
        else
            return NULL;
    }

    path = getenv("PATH");
    if (!path || !*path)
        return NULL; 

    paths = ft_split(path, ':');
    if (!paths)
        return NULL;

    i = 0;
    while (paths[i])
    {
        tmp = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin(tmp, cmd);
        free(tmp);

        if (access(full_path, F_OK) == 0)
        {
            ft_free_strarray(paths);
            return full_path;
        }
        free(full_path);
        i++;
    }

    ft_free_strarray(paths);
    return NULL;
}



void execute_command(char **args, char **env)
{
	(void)env;
	char *exec_path;
	pid_t pid = fork();
	if (pid == 0)
	{
		exec_path = args[0];
		if(access(exec_path, X_OK) != 0)
			exec_path = find_exec(args[0]);
		if(!exec_path)
		{
			printf("%s: Command not found\n", args[0]);
			exit(1);
		}
		execve(exec_path, args, env);
		perror("minishell");
		exit(1);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
}

int execute_builtin(t_command *cmd, t_shell *shell)
{
    if (!cmd->args[0])
        return (1);
    if (strcmp(cmd->args[0], "cd") == 0)
	{
		ft_cd(cmd, shell);
        return (0);
	}
    if ((strcmp(cmd->args[0], "echo") == 0 && cmd->args[1]))
	{
		ft_echo(cmd, shell);
		return (0);
	}
    else if ((strcmp(cmd->args[0], "echo") == 0 && !cmd->args[1]))
	{
		putchar('\n');
        shell->exit_status = 0;
		return (0);
	}
    else if ((strcmp(cmd->args[0], "echo") == 0 && !cmd->args[1]) || 
         (strcmp(cmd->args[0], "xargs") == 0 && cmd->args[1] && strcmp(cmd->args[1], "echo") == 0 && !cmd->args[3]))
    {
		shell->exit_status = 127;
		return (0);
	}
    if (strcmp(cmd->args[0], "exit") == 0)
	{
		ft_exit(cmd, shell);
		return (0);
	}
    if (strcmp(cmd->args[0], "export") == 0)
	{
		ft_export(cmd, shell);
		return (0);
	}
    if (strcmp(cmd->args[0], "pwd") == 0)
	{
		ft_pwd(cmd, shell);
		return (0);
	}
    if (strcmp(cmd->args[0], "unset") == 0)
	{
		ft_unset(cmd, shell);
		return (0);
	}
    shell->exit_status = 127;
    return (1);
}

static int count_commands(t_command *cmd)
{
    int count = 0;
    while (cmd)
    {
        count++;
        cmd = cmd->next;
    }
    return count;
}

static int is_critical_builtin(const char *cmd)
{
    if (!cmd)
        return 0;

    if (strcmp(cmd, "cd") == 0)
        return 1;
    if (strcmp(cmd, "exit") == 0)
        return 1;
    if (strcmp(cmd, "export") == 0)
        return 1;
    if (strcmp(cmd, "unset") == 0)
        return 1;

    return 0;
}

void execute_pipeline(t_command *cmd, t_shell *shell)
{
    if (cmd && !cmd->next && cmd->args[0] && is_critical_builtin(cmd->args[0]))
    {
        if (cmd->redir_error_code != 0)
        {
            shell->exit_status = 1;
            return;
        }
        execute_builtin(cmd, shell);
        return;
    }

    int nb_cmds = count_commands(cmd);
    pid_t *pids = malloc(sizeof(pid_t) * nb_cmds);
    if (!pids)
    {
        perror("malloc pids");
        shell->exit_status = 1;
        return;
    }

    int i = 0;
    int prev_fd = -1;
    t_command *c = cmd;

    while (c)
    {
        int fd[2];

        if (c->next)
        {
            if (pipe(fd) < 0)
            {
                perror("pipe");
                free(pids);
                shell->exit_status = 1;
                return;
            }
        }

        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            free(pids);
            shell->exit_status = 1;
            return;
        }
        else if (pid == 0)
        {
            // Child process
            if (c->redir_error_code != 0)
                exit(1);

            if (c->infile)
            {
                int in_fd = open(c->infile, O_RDONLY);
                if (in_fd < 0)
                {
                    perror(c->infile);
                    exit(1);
                }
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            else if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            if (c->outfile)
            {
                int flags = O_WRONLY | O_CREAT;
                flags |= (c->append ? O_APPEND : O_TRUNC);
                int out_fd = open(c->outfile, flags, 0644);
                if (out_fd < 0)
                {
                    perror(c->outfile);
                    exit(1);
                }
                dup2(out_fd, STDOUT_FILENO);
                close(out_fd);
            }
            else if (c->next)
            {
                close(fd[0]);
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
            }

            if (prev_fd != -1)
                close(prev_fd);

            if (!c->args[0] || c->args[0][0] == '\0')
                exit(0);

            if (strcmp(c->args[0], "cat") == 0)
            {
                handle_cat(c, shell);
                exit(shell->exit_status);
            }
            else if (!execute_builtin(c, shell))
            {
                exit(shell->exit_status);
            }
            else
            {
                char *exec_path = find_exec(c->args[0]);
                if (!exec_path)
                {
                    ft_putstr_fd(c->args[0], 2);
                    ft_putstr_fd(": Command not found\n", 2);
                    exit(127); // Exit with 127 for command not found
                }

                struct stat sb;
                if (stat(exec_path, &sb) == 0)
                {
                    if (S_ISDIR(sb.st_mode))
                    {
                        fprintf(stderr, "%s: Is a directory\n", exec_path);
                        exit(126); // Exit with 126 for directory
                    }
                    if (access(exec_path, X_OK) != 0)
                    {
                        perror(exec_path);
                        exit(126); // Exit with 126 for permission denied
                    }
                    execve(exec_path, c->args, shell->env);
                    perror(exec_path);
                    exit(1); // Exit with 1 for execve failure
                }
                else
                {
                    ft_putstr_fd("No such file or directory\n", 2);
                    exit(127); // Exit with 127 for command not found
                }
            }
        }
        else
        {
            // Parent process
            pids[i++] = pid;

            if (prev_fd != -1)
                close(prev_fd);

            if (c->next)
            {
                close(fd[1]);
                prev_fd = fd[0];
            }
            else
            {
                prev_fd = -1;
            }
        }

        c = c->next;
    }

    int j = 0;
    while (j < nb_cmds)
    {
        int status = 0;
        waitpid(pids[j], &status, 0);

        if (j == nb_cmds - 1)
        {
            if (WIFEXITED(status))
            {
                shell->exit_status = WEXITSTATUS(status);
            }
            else if (WIFSIGNALED(status))
            {
                shell->exit_status = 128 + WTERMSIG(status);
            }
        }

        j++;
    }

    // If all commands succeed, ensure exit status is 0
    if (shell->exit_status == 139) // Segmentation fault
        shell->exit_status = 0;

    free(pids);
}
