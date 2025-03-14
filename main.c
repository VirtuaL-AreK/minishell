#include "minishell.h"

// extern char **environ;
t_shell g_shell = { NULL, 0 };  

int is_syntax_error(t_token *tokens)
{
    if (!tokens)
        return (1);

    while (tokens)
    {
        if (tokens->type == 1)
        {
            if (!tokens->next || tokens->next->type == 1)
            {
                printf("Syntax error near unexpected token '|'\n");
                return 1;
            }
        }
        else if (tokens->type == 3 || tokens->type == 4)
        {
            if (!tokens->next || tokens->next->type != 0)
            {
                printf("Syntax error: missing file for redirection\n");
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

    if (!is_syntax_error(tokens))
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

void prompt_loop(t_shell *shell)
{
    char *input;

    while (1)
    {
        signal(SIGQUIT, SIG_IGN);
        signal(SIGINT, sig_handler);

        input = readline("\033[1;32mminishell$\033[0m ");
        if (!input)
        {
            rl_clear_history();
            exit(0);
        }
        if (*input)
            add_history(input);

        if (!check_unclosed_quotes(input))
            parse_command(input, shell);

        free(input);
    }
}

// NEW TEMPORARY PROMPT LOOP CODE FOR THE TESTER
// void prompt_loop(t_shell *shell)
// {
//     char *input;

//     while (1)
//     {
//         signal(SIGQUIT, SIG_IGN);
//         signal(SIGINT, sig_handler);

//         if (isatty(fileno(stdin)))
//         {
//             input = readline("\033[1;32mminishell$\033[0m ");
//         }
//         else
//         {
//             char *line;
//             line = get_next_line(fileno(stdin));
//             if (!line)
//             {
//                 // Fin de l'entrée non-interactive
//                 exit(0);
//             }
//             input = ft_strtrim(line, "\n");
//             free(line);
//         }

//         if (!input)
//         {
//             rl_clear_history();
//             exit(0);
//         }
//         if (*input)
//             add_history(input);

//         if (!check_unclosed_quotes(input))
//             parse_command(input, shell);

//         free(input);
//     }
// }

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
	return (0);
}
