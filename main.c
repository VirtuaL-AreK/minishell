/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:45:36 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/27 15:50:12 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_last_signal = 0;

void	parse_command(char *input, t_shell *shell)
{
	t_token		*tokens;
	t_command	*commands;

	tokens = bash_tokenize_to_tokens(input);
	if (!tokens)
		return ;
	if (!is_syntax_error(tokens, shell))
	{
		expand_tokens(tokens, shell);
		commands = command_parser(tokens, shell);
		if (shell->heredoc_interrupted)
		{
			free_commands(commands);
			shell->heredoc_interrupted = 0;
			free_tokens(tokens);
			return ;
		}
		execute_pipeline(commands, shell);
		free_commands(commands);
	}
	free_tokens(tokens);
}

void	prompt_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		input = readline("\033[1;32mminishell$\033[0m ");
		if (!input)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			exit(shell->exit_status);
		}
		if (g_last_signal == SIGINT)
		{
			g_last_signal = 0;
			free(input);
			continue ;
		}
		if (*input)
			add_history(input);
		if (!check_unclosed_quotes(input, shell))
			parse_command(input, shell);
		free(input);
	}
}

static char	*read_interactive_input(t_shell *shell)
{
	char	*input;

	input = readline("\033[1;32mminishell$\033[0m ");
	if (!input)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		exit(shell->exit_status);
	}
	if (input[0] == '\0')
	{
		free(input);
		return (NULL);
	}
	return (input);
}

static char	*read_non_interactive_input(t_shell *shell)
{
	char	*line;
	char	*input;

	line = get_next_line(fileno(stdin));
	if (!line)
		exit(shell->exit_status);
	input = ft_strtrim(line, "\n");
	free(line);
	return (input);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.exit_status = 0;
	shell.heredoc_interrupted = 0;
	shell.env = clone_envp(envp, &shell);
	signal(SIGINT, sigint_handler_prompt);
	signal(SIGQUIT, SIG_IGN);
	prompt_loop(&shell);
	free_envp(shell.env);
	return (shell.exit_status);
}
