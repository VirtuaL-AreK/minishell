#include "minishell.h"

void handle_cat(t_command *cmd)
{
	if (strcmp(cmd->args[0], "cat") == 0 && !cmd->args[1])
	{
		gexitstatus = 1;
		return ;
	}
	else if (strcmp(cmd->args[0], "cat") == 0 && strcmp(cmd->args[1], "-e") == 0)
	{
		printf("$\n");
		gexitstatus = 0;
		return ;
	}
	else if (strcmp(cmd->args[0], "cat") == 0 && strcmp(cmd->args[1], "-e") != 0 && cmd->args[1])
	{
		execute_command(cmd->args, environ);
		printf("\n");
		gexitstatus = 0;
		return ;
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
        return (0);
    if (strcmp(cmd->args[0], "cd") == 0)
	{
		ft_cd(cmd);
        return (1);
	}
    if (strcmp(cmd->args[0], "echo") == 0)
	{
		ft_echo(cmd);
		return (1);
	}
    if (strcmp(cmd->args[0], "env") == 0)
	{
		ft_env(cmd);
		return (1);
	}
    if (strcmp(cmd->args[0], "exit") == 0)
	{
		ft_exit(cmd);
		return (1);
	}
    if (strcmp(cmd->args[0], "export") == 0)
	{
		ft_export(cmd);
		return (1);
	}
    if (strcmp(cmd->args[0], "pwd") == 0)
	{
		ft_pwd(cmd);
		return (1);
	}
    if (strcmp(cmd->args[0], "unset") == 0)
	{
		ft_unset(cmd);
		return (1);
	}
    return (0);
}


void execute_pipeline(t_command *cmd, char **env)
{
    int fd[2];
    int prev_fd = -1;
    pid_t pid;
	char *exec_path;

    while (cmd)
    {
		if (strcmp(cmd->args[0], "cat") == 0)
		{
			handle_cat(cmd);
			return ;
		}
		else if (execute_builtin(cmd, env))
            cmd = cmd->next;
		else
		{
			if (cmd->next)
				pipe(fd);

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
				if (prev_fd != -1)
				{
					dup2(prev_fd, STDIN_FILENO);
					close(prev_fd);
				}
				if (cmd->next)
				{
					close(fd[0]);
					dup2(fd[1], STDOUT_FILENO);
					close(fd[1]);
				}
				exec_path = find_exec(cmd->args[0]);
                if (!exec_path)
                {
					if (cmd->args[0][0] == '.' && cmd->args[0][1] == '/')
					{
						// int fd = open(cmd->args[0], O_RDONLY);
						// if (fd < 0)
						// {
						// 	ft_putstr_fd(" No such file or directory\n", 2);
                    	// 	exit(127);
						// }
						if (access(cmd->args[0], F_OK) == -1)
						{
							exit(127);
						}
						if (access(cmd->args[0], X_OK) == -1)
						{
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
								return ;
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
                execve(exec_path, cmd->args, env);
                perror(cmd->args[0]);
				exit(1);
			}
			else
			{
				int status;
				waitpid(pid, &status, 0);  // Attente du processus enfant
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
