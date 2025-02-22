#include "minishell.h"

void parse_command(char *input, char **env)
{
	int i = 0;
	t_command *commands;
	(void)env;

	char **args; // Parsing to do
	t_token *tokens;
	args = ft_split(input, ' ');
	if (!args || !args[0])
		return;

	if (syntax_verification(args))
	{
		tokens = tokenization(args);
		printf(GREEN "Good syntax\n" RESET);
	}
	else
		printf(RED "Bad Syntax!\n" RESET);
	
	commands = tokens_to_commands(tokens);
	print_command(commands);
	//execute_command(args, env);
	
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
