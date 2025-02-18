#include "minishell.h"

void execute_command(char **args, char **env)
{
	(void)env;
	pid_t pid = fork();
	if (pid == 0) // Child process
	{
		execve(args[0], args, env);
		//execvp(args[0], args);
		printf("Command not found\n");
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
    prompt_loop(env);
}
