/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 09:40:08 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/18 18:21:59 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h" 

int count_command_arguments(t_token *tokens)
{
    int count = 0;
    while (tokens && tokens->type != TOKEN_PIPE)
    {
        if (tokens->type == TOKEN_REDIR_IN
         || tokens->type == TOKEN_REDIR_OUT
         || tokens->type == TOKEN_APPEND
         || tokens->type == TOKEN_HEREDOC)
        {
            tokens = tokens->next;
            if (tokens)
                tokens = tokens->next;
        }
        else if (tokens->type == TOKEN_WORD)
        {
            count++;
            tokens = tokens->next;
        }
        else
            tokens = tokens->next;
    }
    return count;
}

t_command	*new_command(t_token *tokens)
{
	t_command	*cmd;
	int			nb_arg;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	nb_arg = count_command_arguments(tokens);
	cmd->nb_arg = nb_arg;
	cmd->args = malloc(sizeof(char *) * (nb_arg + 1));
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->redir_error_code = 0;
	cmd->next = NULL;
	return (cmd);
}


int handle_heredoc_line(t_heredoc_ctx *ctx, char *line)
{
    int cmp;
    char *expanded_line;

    cmp = strcmp(line, ctx->delimiter);
    if (cmp == 0)
        return 1;
    if (!ctx->is_quoted)
    {
        expanded_line = expand_heredoc_line(line, ctx->shell);
        write(ctx->fd, expanded_line, ft_strlen(expanded_line));
        free(expanded_line);
    }
    else
        write(ctx->fd, line, ft_strlen(line));
    write(ctx->fd, "\n", 1);
    return 0;
}

// void	sigint_handler_heredoc(int sig)
// {
// 	(void)sig;
// 	g_last_signal = SIGINT;
// 	rl_done = 1;
// 	{
// 		char	nl;

// 		nl = '\n';
// 		ioctl(STDIN_FILENO, TIOCSTI, &nl);
// 	}
// }

static void sigint_handler_heredoc(int sig)
{
    (void)sig;
    g_last_signal = SIGINT;
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_done = 1;
    {
        char nl = '\n';
        ioctl(STDIN_FILENO, TIOCSTI, &nl);
    }
}

// void init_heredoc_signals(struct sigaction *old_sa)
// {
//     struct sigaction sa;

//     ft_memset(&sa, 0, sizeof(sa));
//     sa.sa_handler = sigint_handler_heredoc;
//     sigemptyset(&sa.sa_mask);
//     sa.sa_flags = 0;
//     sigaction(SIGINT, &sa, old_sa);
//     signal(SIGQUIT, SIG_IGN);
// }

static void init_heredoc_signals(struct sigaction *old_sa)
{
    struct sigaction sa;

    ft_memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigint_handler_heredoc;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, old_sa);
    signal(SIGQUIT, SIG_IGN);
}

// void restore_heredoc_signals(struct sigaction *old_sa)
// {
//     sigaction(SIGINT, old_sa, NULL);
//     signal(SIGQUIT, SIG_DFL);
// }

static void restore_heredoc_signals(struct sigaction *old_sa)
{
    sigaction(SIGINT, old_sa, NULL);
    signal(SIGQUIT, SIG_DFL);
}

int create_heredoc_file(char *template)
{
    int fd;

    fd = mkstemp(template);
    if (fd < 0)
    {
        ft_putstr_fd("mkstemp error\n", 2);
        return -1;
    }
    return fd;
}

// int read_heredoc_and_write(t_heredoc_ctx *ctx)
// {
//     char             *line;
//     int               status;
//     struct sigaction  old_sa;

//     init_heredoc_signals(&old_sa);

//     while (1)
//     {
//         line = readline("> ");
//         if (!line || g_last_signal == SIGINT)
//             break;
//         status = handle_heredoc_line(ctx, line);
//         free(line);
//         if (status != 0)
//             break;
//     }

//     restore_heredoc_signals(&old_sa);

//     return (g_last_signal == SIGINT);
// }

int read_heredoc_and_write(t_heredoc_ctx *ctx)
{
    struct sigaction old_sa;
    char           *line;
    int             status;

    init_heredoc_signals(&old_sa);
    while (1)
    {
        line = readline("> ");
        if (!line || g_last_signal == SIGINT)
            break;
        status = handle_heredoc_line(ctx, line);
        free(line);
        if (status != 0)
            break;
    }
    restore_heredoc_signals(&old_sa);
    return (g_last_signal == SIGINT);
}

// char *handle_heredoc(const char *delimiter, int is_quoted, t_shell *shell)
// {
//     t_heredoc_ctx ctx;
//     char         *template = ft_strdup("/tmp/minishell_heredoc_XXXXXX");
//     int           fd, status;

//     fd = mkstemp(template);
//     if (fd < 0)
//     {
//         free(template);
//         return NULL;
//     }

//     ctx.fd        = fd;
//     ctx.is_quoted = is_quoted;
//     ctx.delimiter = delimiter;
//     ctx.shell     = shell;

//     status = read_heredoc_and_write(&ctx);
//     close(fd);

//     if (status != 0)
//     {
//         shell->exit_status = 130; 
//         free(template);
//         return NULL;
//     }
//     return template;
// }


char *handle_heredoc(const char *delimiter,
                     int is_quoted,
                     t_shell *shell)
{
    t_heredoc_ctx    ctx;
    char            *template;
    int              fd;
    int              interrupted;

    template = ft_strdup("/tmp/minishell_heredoc_XXXXXX");
    fd = mkstemp(template);
    if (fd < 0)
    {
        free(template);
        return (NULL);
    }
    ctx.fd         = fd;
    ctx.is_quoted  = is_quoted;
    ctx.delimiter  = delimiter;
    ctx.shell      = shell;

    interrupted = read_heredoc_and_write(&ctx);
    close(fd);
    if (interrupted)
    {
        shell->exit_status         = 130;
        shell->heredoc_interrupted = 1;
        g_last_signal              = 0;
        free(template);
        return (NULL);
    }
    return (template);
}

void	handle_word(t_command *cmd, t_token **tokens, int *arg_count)
{
	cmd->args[(*arg_count)++] = ft_strdup((*tokens)->value);
	*tokens = (*tokens)->next;
}
