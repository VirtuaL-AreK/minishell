#include "minishell.h"

// extern char **environ;

int is_syntax_error(t_token *tokens)
{
	if (!tokens)
		return (1);
	if (tokens && tokens->type == 1)
		return (printf("Syntax error: unexpected '|'\n"), 1);
    while (tokens)
    {
        if (tokens->type == 1 && (!tokens->next || tokens->next->type == 1))
            return (printf("Syntax error near unexpected token '|'\n"), 1);
        if ((tokens->type == 1 || tokens->type == 3)
            && (!tokens->next || tokens->next->type != 0))
            return (printf("Syntax error: missing file for redirection\n"), 1);
        tokens = tokens->next;
    }
    return (0);
}

void parse_command(char *input, char **env)
{
	t_command *commands;

	char **args; // Parsing to do
	t_token *tokens;
	args = ft_split(input, ' ');
	if (!args || !args[0])
		return;

	// if (syntax_verification(args))
	// {
		tokens = tokenization(args);
	// 	printf(GREEN "Good syntax\n" RESET);
	// }
	// else
	// 	printf(RED "Bad Syntax!\n" RESET);
	
	if(!is_syntax_error(tokens))
	{
		commands = command_parser(tokens);
		print_command(commands);
		// if (strcmp(args[0], "echo") == 0)
		// 	ft_echo(commands);
		// else if (strcmp(args[0], "cd") == 0)
		// 	ft_cd(commands);
		// else if (strcmp(args[0], "export") == 0)
		// 	ft_export(commands);
		// else if (strcmp(args[0], "unset") == 0)
		// 	ft_unset(commands);
		// else if (strcmp(args[0], "env") == 0)
		// 	ft_env(commands);
		// else if (strcmp(args[0], "exit") == 0)
		// 	ft_exit(commands);
		// else if (strcmp(args[0], "pwd") == 0)
		// 	ft_pwd(commands);
		// else
			execute_pipeline(commands, env);
	}
	
	ft_free_args(args);
	free_tokens(tokens);
	free_commands(commands);
}

void prompt_loop(char **env)
{
	char *input;

	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, sig_handler);
		input = readline("\033[1;32mminishell$\033[0m ");
		if (!input)
		{
			printf("exit\n");
			rl_clear_history();
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

	//printf("%s\n", getenv("PATH"));
    prompt_loop(env);
	return (0);
}
