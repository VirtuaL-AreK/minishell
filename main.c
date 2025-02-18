#include "minishell.h"


char *find_exec(char *cmd, char **env)
{
	(void)env;
    char *path = getenv("PATH");
    char **paths = ft_split(path, ':');
    char *full_path;
    int i = 0;

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
	char *exec_path;
	(void)env;
	pid_t pid = fork();
	if (pid == 0) // Child process
	{
		exec_path = args[0];
		if(access(exec_path, X_OK) != 0)
			exec_path = find_exec(args[0], env);
		if(!exec_path)
		{
			printf("Command not found\n");
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

void parse_command(char *input, char **env)
{
	int i = 0;

	char **args = ft_split(input, ' '); // Parsing to do
	if (!args || !args[0])
		return;
	
	execute_command(args, env);
	
	while(args[i])
	{
		free(args[i]);
		i++;
	}
		
	free(args);
}

void prompt_loop(char **env)
{
	char *input;

	while (1)
	{
		input = readline("\033[1;32mminishell$\033[0m ");
		if (!input)
		{
			printf("exit\n");
			break;
		}
		if (*input)
			add_history(input);
		parse_command(input, env);
		//printf("You entered : %s\n", input);
		free(input);
	}
}

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	//(void)env;

	//printf("%s\n", getenv("PATH"));
    prompt_loop(env);
}
