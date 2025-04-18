#include "minishell.h"

// extern char **environ;
// t_shell g_shell = { NULL, 0 , '\0', 0};

volatile sig_atomic_t g_last_signal = 0;

void parse_command(char *input, t_shell *shell)
{
    t_token *tokens = bash_tokenize_to_tokens(input);
	// print_tokens(tokens);
    if (!tokens)
        return;

    if (!is_syntax_error(tokens, shell))
    {
        expand_tokens(tokens, shell);
        // t_command *commands = command_parser(tokens);
        t_command *commands = command_parser(tokens, shell);
		// print_command(commands);
		execute_pipeline(commands, shell);
        free_commands(commands);
    }
    free_tokens(tokens);
}

// KEEP THIS CODE BELOW

// void	prompt_loop(t_shell *shell)
// {
// 	char	*input;

// 	while (1)
// 	{
// 		input = readline("\033[1;32mminishell$\033[0m ");
// 		if (!input)
// 		{
// 			write(STDOUT_FILENO, "exit\n", 5);
// 			exit(shell->exit_status);
// 		}
// 		if (g_last_signal == SIGINT)
// 		{
// 			g_last_signal = 0;
// 			free(input);
// 			continue ;
// 		}
// 		if (*input)
// 			add_history(input);
// 		if (!check_unclosed_quotes(input, shell))
// 			parse_command(input, shell);
// 		free(input);
// 	}
// }

// NEW TEMPORARY PROMPT LOOP CODE FOR THE TESTER

void	prompt_loop(t_shell *shell)
{
	char	*input;
	char	*line;

	while (1)
	{
		if (isatty(fileno(stdin)))
		{
			input = readline("\033[1;32mminishell$\033[0m ");
			if (!input)
			{
				write(STDOUT_FILENO, "exit\n", 5);
				exit(shell->exit_status);
			}
			if (input[0] == '\0')
			{
				free(input);
				continue ;
			}
		}
		else
		{
			line = get_next_line(fileno(stdin));
			if (!line)
				exit(shell->exit_status);
			input = ft_strtrim(line, "\n");
			free(line);
		}

		if (*input)
			add_history(input);

		if (!check_unclosed_quotes(input, shell))
			parse_command(input, shell);

		free(input);
	}
}


int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.exit_status = 0;
	shell.env = clone_envp(envp, &shell);

	signal(SIGINT, sigint_handler_prompt);
	signal(SIGQUIT, SIG_IGN);

	prompt_loop(&shell);

	free_envp(shell.env);
	return (shell.exit_status);
}