#include "minishell.h"

char *find_exec(char *cmd)
{
    char *path;
    char **paths;
    char *full_path;
    int i;

	i = 0;
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
	if (pid == 0) // Child process
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
