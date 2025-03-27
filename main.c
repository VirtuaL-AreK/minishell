#include "minishell.h"

// extern char **environ;
t_shell g_shell = { NULL, 0 };  

int is_syntax_error(t_token *tokens, t_shell *shell)
{
    if (!tokens)
    {
        shell->exit_status = 2;
        return (1);
    }
	if (tokens->type == TOKEN_PIPE)
        {
            ft_putstr_fd("Syntax error near unexpected token '|'\n",2);
			shell->exit_status = 2;
            return 1;
        }
    while (tokens)
    {
        if (tokens->type == TOKEN_PIPE)
        {
            if (!tokens->next || tokens->next->type == TOKEN_PIPE)
            {
                ft_putstr_fd("Syntax error near unexpected token '|'\n",2);
				shell->exit_status = 2;
                return 1;
            }
        }
        else if (tokens->type == TOKEN_REDIR_IN || tokens->type == TOKEN_REDIR_OUT ||
                 tokens->type == TOKEN_APPEND || tokens->type == TOKEN_HEREDOC)
        {
            if (!tokens->next || tokens->next->type != TOKEN_WORD)
            {
                if (tokens->type == TOKEN_HEREDOC)
                   	ft_putstr_fd("Syntax error: missing delimiter for heredoc\n", 2);
                else
                    ft_putstr_fd("Syntax error: missing file for redirection\n", 2);
                shell->exit_status = 2;
                return 1;
            }
        }
        tokens = tokens->next;
    }
    return 0;
}



void parse_command(char *input, t_shell *shell)
{
    t_token *tokens = bash_tokenize_to_tokens(input);
	// print_tokens(tokens);
    if (!tokens)
        return;

    if (!is_syntax_error(tokens, shell))
    {
        expand_tokens(tokens, shell);
        t_command *commands = command_parser(tokens);
        // print_command(commands);
		execute_pipeline(commands, shell);
        free_commands(commands);
    }
    free_tokens(tokens);
}

// KEEP THIS CODE BELOW

// void prompt_loop(t_shell *shell)
// {
//     char *input;

//     while (1)
//     {
//         signal(SIGQUIT, SIG_IGN);
//         signal(SIGINT, sig_handler);

//         input = readline("\033[1;32mminishell$\033[0m ");
//         if (!input)
//         {
//             rl_clear_history();
//             exit(shell->exit_status);
//         }
//         if (*input)
//             add_history(input);

//         if (!check_unclosed_quotes(input))
//             parse_command(input, shell);

//         free(input);
//     }
// }

// NEW TEMPORARY PROMPT LOOP CODE FOR THE TESTER
void prompt_loop(t_shell *shell)
{
    char *input;

    while (1)
    {
        signal(SIGQUIT, SIG_IGN);
        signal(SIGINT, sig_handler);

        if (isatty(fileno(stdin)))
        {
            input = readline("\033[1;32mminishell$\033[0m ");
        }
        else
        {
            char *line;
            line = get_next_line(fileno(stdin));
            if (!line)
            {
                // Fin de l'entrée non-interactive
                exit(shell->exit_status);
            }
            input = ft_strtrim(line, "\n");
            free(line);
        }

        if (!input)
        {
            rl_clear_history();
            exit(shell->exit_status);
        }
        if (*input)
            add_history(input);

        if (!check_unclosed_quotes(input, shell))
            parse_command(input, shell);

        free(input);
    }
}

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;

	g_shell.env = clone_envp(env);
    g_shell.exit_status = 0;

    // On lance la boucle
    prompt_loop(&g_shell);

    // Si on sort de la boucle (par "exit" ou Ctrl-D)
    free_envp(g_shell.env);
	return (g_shell.exit_status);
}
