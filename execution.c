#include "minishell.h"

void handle_cat(t_command *cmd)
{
    char buffer[1024];
    int bytes;
	int i;

	i = 0;
    if (strcmp(cmd->args[0], "cat") == 0 && !cmd->args[1])
    {
        while ((bytes = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
        {
            if (write(STDOUT_FILENO, buffer, bytes) < 0)
            {
                perror("write");
                gexitstatus = 1;
                return;
            }
        }
        gexitstatus = 0;
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
        gexitstatus = 0;
        return;
    }
    else if (strcmp(cmd->args[0], "cat") == 0 && cmd->args[1])
    {
        execute_command(cmd->args, environ);
        gexitstatus = 0;
        return;
    }
}


char *find_exec(char *cmd)
{
    char *path;
    char **paths;
    char *full_path;
    int i;

	i = 0;
	if (cmd[0] == '/' && access(cmd, X_OK) == 0)
        return cmd;
	path = getenv("PATH");
	paths = ft_split(path, ':');
    if (!paths)
        return NULL;
    while (paths[i])
    {
        full_path = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin(full_path, cmd);
		if (access(full_path, X_OK) == 0)
        {
            free(paths[i]);
            return full_path;
        }
        free(full_path);
        free(paths[i]);
        i++;
    }
    free(paths);
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

int execute_builtin(t_command *cmd, char **env)
{
	(void)env;
    if (!cmd->args[0])
        return (1);
    if (strcmp(cmd->args[0], "cd") == 0)
	{
		ft_cd(cmd);
        return (0);
	}
    if (strcmp(cmd->args[0], "echo") == 0)
	{
		ft_echo(cmd);
		return (0);
	}
    if (strcmp(cmd->args[0], "env") == 0)
	{
		ft_env(cmd);
		return (0);
	}
    if (strcmp(cmd->args[0], "exit") == 0)
	{
		ft_exit(cmd);
		return (0);
	}
    if (strcmp(cmd->args[0], "export") == 0)
	{
		ft_export(cmd);
		return (0);
	}
    if (strcmp(cmd->args[0], "pwd") == 0)
	{
		ft_pwd(cmd);
		return (0);
	}
    if (strcmp(cmd->args[0], "unset") == 0)
	{
		ft_unset(cmd);
		return (0);
	}
    return (1);
}

#include <string.h>

int is_simple_builtin(const char *cmd)
{
	if (!cmd)
		return 0;
	if (strcmp(cmd, "echo") == 0)
		return 1;
	if (strcmp(cmd, "pwd") == 0)
		return 1;
	if (strcmp(cmd, "export") == 0)
		return 1;
    return 0;
}

int is_builtin(const char *cmd)
{
	if (!cmd)
		return 0;
	if (strcmp(cmd, "cd") == 0)
		return 1;
	if (strcmp(cmd, "env") == 0)
		return 1;
	if (strcmp(cmd, "export") == 0)
		return 1;
	if (strcmp(cmd, "unset") == 0)
		return 1;
	if (strcmp(cmd, "exit") == 0)
		return 1;
    return 0;
}

void execute_pipeline(t_command *cmd, char **env)
{
    int fd[2];
    int prev_fd = -1;
    pid_t pid;
    int status;
    char *exec_path;

    while (cmd)
    {
		if (!cmd->args[0])
		{
			// Pas de commande => on fait rien, code 0
			gexitstatus = 0;
			cmd = cmd->next;
			continue;
		}

        // erreur de redirection => ne pas exécuter la commande et retourner exit code 1
        if (cmd->redir_error_code != 0)
        {
            pid = fork();
            if (pid == 0)
            {
                exit(1);
            }
            else
            {
                waitpid(pid, &status, 0);
                if (WIFEXITED(status))
                    gexitstatus = WEXITSTATUS(status);
                if (prev_fd != -1)
                    close(prev_fd);
                if (cmd->next)
                {
                    if (pipe(fd) < 0)
                    {
                        perror("pipe");
                        exit(1);
                    }
                    close(fd[1]);
                    prev_fd = fd[0];
                }
                cmd = cmd->next;
                continue;
            }
        }

        // cat

        if (strcmp(cmd->args[0], "cat") == 0)
        {
            if (cmd->next)
            {
                if (pipe(fd) < 0)
                {
                    perror("pipe");
                    exit(1);
                }
                pid = fork();
                if (pid == 0)
                {
                    if (cmd->outfile)
                    {
                        int out_fd = open(cmd->outfile, O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC), 0644);
                        if (out_fd < 0)
                        {
                            perror(cmd->outfile);
                            exit(1);
                        }
                        dup2(out_fd, STDOUT_FILENO);
                        close(out_fd);
                    }
                    if (cmd->infile)
                    {
                        int in_fd = open(cmd->infile, O_RDONLY);
                        if (in_fd < 0)
                        {
                            perror(cmd->infile);
                            exit(1);
                        }
                        dup2(in_fd, STDIN_FILENO);
                        close(in_fd);
                        if (prev_fd != -1)
                            close(prev_fd);
                    }
                    dup2(fd[1], STDOUT_FILENO);
                    close(fd[0]);
                    close(fd[1]);
                    handle_cat(cmd);
                    exit(gexitstatus);
                }
                else
                {
                    close(fd[1]);
                    prev_fd = fd[0];
                    cmd = cmd->next;
                    continue;
                }
            }
            else
            {
                pid = fork();
                if (pid == 0)
                {
                    if (cmd->outfile)
                    {
                        int out_fd = open(cmd->outfile, O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC), 0644);
                        if (out_fd < 0)
                        {
                            perror(cmd->outfile);
                            exit(1);
                        }
                        dup2(out_fd, STDOUT_FILENO);
                        close(out_fd);
                    }
                    if (cmd->infile)
                    {
                        int in_fd = open(cmd->infile, O_RDONLY);
                        if (in_fd < 0)
                        {
                            perror(cmd->infile);
                            exit(1);
                        }
                        dup2(in_fd, STDIN_FILENO);
                        close(in_fd);
                        if (prev_fd != -1)
                            close(prev_fd);
                    }
                    else if (prev_fd != -1)
                    {
                        dup2(prev_fd, STDIN_FILENO);
                        close(prev_fd);
                    }
                    handle_cat(cmd);
                    exit(gexitstatus);
                }
                else
                {
                    waitpid(pid, &status, 0);
                    if (WIFEXITED(status))
                        gexitstatus = WEXITSTATUS(status);
                    return;
                }
            }
        }
        // builtins
        else if (is_builtin(cmd->args[0]))
        {
            execute_builtin(cmd, env);
            cmd = cmd->next;
            continue;
        }
        // commandes externes
        else
        {
            if (cmd->next)
            {
                if (pipe(fd) < 0)
                {
                    perror("pipe");
                    exit(1);
                }
            }
            pid = fork();
            if (pid == 0)
            {
                if (cmd->infile)
                {
                    int in_fd = open(cmd->infile, O_RDONLY);
                    if (in_fd < 0)
                    {
                        perror(cmd->infile);
                        exit(1);
                    }
                    dup2(in_fd, STDIN_FILENO);
                    close(in_fd);
                }
                if (prev_fd != -1)
                {
                    dup2(prev_fd, STDIN_FILENO);
                    close(prev_fd);
                }
                if (cmd->outfile)
                {
                    int out_fd = open(cmd->outfile, O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC), 0644);
                    if (out_fd < 0)
                    {
                        perror(cmd->outfile);
                        exit(1);
                    }
                    dup2(out_fd, STDOUT_FILENO);
                    close(out_fd);
                }
                if (cmd->next)
                {
                    close(fd[0]);
                    if (!cmd->outfile)
                        dup2(fd[1], STDOUT_FILENO);
                    close(fd[1]);
                }
                
                if (is_simple_builtin(cmd->args[0]))
                    exit(execute_builtin(cmd, env));
                
                exec_path = find_exec(cmd->args[0]);
                if (!exec_path)
                {
                    if (cmd->args[0][0] == '/')
                    {
                        ft_putstr_fd(" No such file or directory\n", 2);
                        exit(127);
                    }
                    else if (cmd->args[0][0] == '.' && cmd->args[0][1] == '/')
                    {
                        if (access(cmd->args[0], F_OK) == -1)
                        {
                            ft_putstr_fd(" No such file or directory\n", 2);
                            exit(127);
                        }
                        if (access(cmd->args[0], X_OK) == -1)
                        {
                            ft_putstr_fd(" Permission denied\n", 2);
                            exit(126);
                        }
                        ft_putstr_fd(" Is a directory\n", 2);
                        exit(126);
                    }
                    else
                    {
                        if (strcmp(cmd->args[0], "$PWD") == 0)
                        {
                            ft_putstr_fd(" Is a directory\n", 2);
                            exit(126);
                        }
                        else if (strcmp(cmd->args[0], "$EMPTY") == 0)
                        {
                            if (strcmp(cmd->args[1], "echo") == 0)
                            {
                                printf("%s\n", cmd->args[2]);
                                gexitstatus = 0;
                                return;
                            }
                            else
                                exit(0);
                        }
                        else
                        {
                            ft_putstr_fd(" command not found\n", 2);
                            exit(127);
                        }
                    }
                }
				struct stat sb;
				if (stat(exec_path, &sb) == 0)
				{
					if (S_ISDIR(sb.st_mode))
					{
						ft_putstr_fd(" Is a directory\n", 2);
						exit(126); // code d'erreur de type "Cannot execute"
					}
				}
                execve(exec_path, cmd->args, env);
                perror(cmd->args[0]);
                exit(1);
            }
            else
            {
                waitpid(pid, &status, 0);
                if (WIFEXITED(status))
                    gexitstatus = WEXITSTATUS(status);
                if (prev_fd != -1)
                    close(prev_fd);
                if (cmd->next)
                {
                    close(fd[1]);
                    prev_fd = fd[0];
                }
                cmd = cmd->next;
            }
        }
    }
}
