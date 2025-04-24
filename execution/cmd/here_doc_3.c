#include "../../minishell.h"

int handle_heredoc_failure(char  *tmp, t_command **pcmd, t_token  **ptokens, t_shell   *shell)
{
    if (!tmp)
    {
        (*pcmd)->redir_error_code = 1;
        if (shell->heredoc_interrupted)
            return (1);
        *ptokens = (*ptokens)->next;
        return (1);
    }
    return (0);
}

void handle_heredoc_token(t_command *cmd, t_token   **tokens, t_shell    *shell)
{
    int   is_quoted;
    char *clean;
    char *effective;
    char *tmp;

    clean = get_heredoc_clean(tokens, cmd, &is_quoted);
    if (!clean)
        return ;
    effective = get_effective_delim(clean, is_quoted, shell);
    free(clean);

    tmp = handle_heredoc(effective, is_quoted, shell);
    free(effective);

    if (handle_heredoc_failure(tmp, &cmd, tokens, shell))
        return ;
    free(cmd->infile);
    cmd->infile = tmp;
    *tokens     = (*tokens)->next;
}

void handle_redir_out_or_append(t_command *cmd, t_token **tokens)
{
    int flags;
    int out_fd;
    int is_append = ((*tokens)->type == TOKEN_APPEND);

    *tokens = (*tokens)->next;
    flags = O_WRONLY | O_CREAT | (is_append ? O_APPEND : O_TRUNC);
    out_fd = open((*tokens)->value, flags, 0644);
    if (out_fd < 0)
    {
        print_error((*tokens)->value);
        cmd->redir_error_code = 1;
        while (*tokens && (*tokens)->type != TOKEN_PIPE)
            *tokens = (*tokens)->next;
        return;
    }
    close(out_fd);
    free(cmd->outfile);
    cmd->outfile = ft_strdup((*tokens)->value);
    cmd->append = is_append;
    *tokens = (*tokens)->next;
}

void fill_command(t_command *cmd, t_token **tokens, t_shell *shell)
{
    int arg_count = 0;
    while (*tokens && (*tokens)->type != TOKEN_PIPE)
    {
        if ((*tokens)->type == TOKEN_WORD)
            handle_word(cmd, tokens, &arg_count);
        else if ((*tokens)->type == TOKEN_REDIR_IN && (*tokens)->next)
            handle_redir_in(cmd, tokens);
        else if ((*tokens)->type == TOKEN_HEREDOC)
            handle_heredoc_token(cmd, tokens, shell);
        else if ((*tokens)->type == TOKEN_REDIR_OUT
              || (*tokens)->type == TOKEN_APPEND)
            handle_redir_out_or_append(cmd, tokens);
        else
            *tokens = (*tokens)->next;
    }
    cmd->args[arg_count] = NULL;
}

void	fix_empty_first_arg(t_command *cmd)
{
	int	i;

	if (!cmd->args[0])
		return ;
	if (cmd->args[0][0] == '\0')
	{
		if (!cmd->args[1])
		{
			free(cmd->args[0]);
			cmd->args[0] = NULL;
			cmd->nb_arg = 0;
		}
		else
		{
			free(cmd->args[0]);
			i = 0;
			while (cmd->args[i + 1])
			{
				cmd->args[i] = cmd->args[i + 1];
				i++;
			}
			cmd->args[i] = NULL;
			cmd->nb_arg--;
		}
	}
}
