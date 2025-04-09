/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 09:40:08 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/09 18:54:16 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h" 

int	count_command_arguments(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != 1)
	{
		if (tokens->type == 2 || tokens->type == 3 || tokens->type == 4)
		{
			tokens = tokens->next;
			if (tokens)
				tokens = tokens->next;
		}
		else if (tokens->type == 0)
		{
			count++;
			tokens = tokens->next;
		}
		else
		{
			tokens = tokens->next;
		}
	}
	return (count);
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

static void sigint_handler_heredoc(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    g_shell.exit_status = 130;
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_done = 1;
    g_shell.heredoc_interrupted = 1;
	char newline = 32;
    ioctl(STDIN_FILENO, TIOCSTI, &newline);
}


static void init_heredoc_signals(struct sigaction *old_sa)
{
    struct sigaction sa;

    ft_memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &sigint_handler_heredoc;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, old_sa);
    signal(SIGQUIT, SIG_IGN);
    g_shell.heredoc_interrupted = 0;
}

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

int read_heredoc_and_write(t_heredoc_ctx *ctx)
{
    char                *line;
    int                 status;
    struct sigaction    old_sa;

    init_heredoc_signals(&old_sa);
    while (1)
    {
        line = readline("> ");
        if (!line || g_shell.heredoc_interrupted)
            break;
        status = handle_heredoc_line(ctx, line);
        free(line);
        if (status != 0)
            break;
    }
    restore_heredoc_signals(&old_sa);
    if (!line || g_shell.heredoc_interrupted)
        return (1);
    return (0);
}


char *handle_heredoc(const char *delimiter, int is_quoted, t_shell *shell)
{
    t_heredoc_ctx ctx;
    char *template;
    int fd;
    int status;

    template = ft_strdup("/tmp/minishell_heredoc_XXXXXX");
    fd = create_heredoc_file(template);
    if (fd < 0)
    {
        free(template);
        return NULL;
    }
    ctx.fd = fd;
    ctx.is_quoted = is_quoted;
    ctx.delimiter = delimiter;
    ctx.shell = shell;
    status = read_heredoc_and_write(&ctx);
    close(fd);
    if (status != 0)
    {
        free(template);
        return NULL;
    }
    return ft_strdup(template);
}



void	handle_word(t_command *cmd, t_token **tokens, int *arg_count)
{
	cmd->args[(*arg_count)++] = ft_strdup((*tokens)->value);
	*tokens = (*tokens)->next;
}
