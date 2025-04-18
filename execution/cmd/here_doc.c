/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 20:28:14 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/18 18:22:35 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char *ft_strjoin_and_free(char *s1, const char *s2)
{
    if (!s1 && !s2)
        return NULL;
    if (!s1)
        return ft_strdup(s2);
    if (!s2)
        return s1;

    char *new_str;
    size_t len1 = ft_strlen(s1);
    size_t len2 = ft_strlen(s2);

    new_str = malloc(len1 + len2 + 1);
    if (!new_str)
        return (free(s1), NULL);
    ft_memcpy(new_str, s1, len1);
    ft_memcpy(new_str + len1, s2, len2);
    new_str[len1 + len2] = '\0';
    free(s1);
    return new_str;
}

void append_char_result(char c, char **result)
{
    char temp[2];

    temp[0] = c;
    temp[1] = '\0';
    *result = ft_strjoin_and_free(*result, temp);
}


int expand_variable_name(const char *line, int i,
    char **result, t_shell *shell)
{
    int start_var;
    int var_len;
    char *var_name;
    char *env_val;

    start_var = i;
    while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
        i++;
    var_len = i - start_var;
    if (var_len > 0)
    {
        var_name = ft_substr(line, start_var, var_len);
        env_val = get_local_env_value(var_name, shell);
        *result = ft_strjoin_and_free(*result, env_val);
        free(var_name);
        free(env_val);
    }
    else
        *result = ft_strjoin_and_free(*result, "$");
    return i;
}


void expand_exit_status(char **result, t_shell *shell)
{
    char *exit_str;

    exit_str = ft_itoa(shell->exit_status);
    *result = ft_strjoin_and_free(*result, exit_str);
    free(exit_str);
}


int process_dollar_in_heredoc(const char *line, int i,
    char **result, t_shell *shell)
{
    i++;
    if (line[i] == '?')
    {
        i++;
        expand_exit_status(result, shell);
    }
    else
        i = expand_variable_name(line, i, result, shell);
    return i;
}


char *expand_heredoc_line(const char *line, t_shell *shell)
{
    char *result;
    int i;

    result = ft_strdup("");
    i = 0;
    while (line[i])
    {
        if (line[i] == '$')
            i = process_dollar_in_heredoc(line, i, &result, shell);
        else
        {
            append_char_result(line[i], &result);
            i++;
        }
    }
    return result;
}



void	handle_redir_in(t_command *cmd, t_token **tokens)
{
	int	in_fd;

	*tokens = (*tokens)->next;
	in_fd = open((*tokens)->value, O_RDONLY);
	if (in_fd < 0)
	{
		ft_putstr_fd((*tokens)->value, 2);
		cmd->redir_error_code = 1;
		while (*tokens && (*tokens)->type != TOKEN_PIPE)
			*tokens = (*tokens)->next;
		return ;
	}
	close(in_fd);
	free(cmd->infile);
	cmd->infile = ft_strdup((*tokens)->value);
	*tokens = (*tokens)->next;
}

char *remove_surrounding_quotes_if_any(const char *str)
{
    size_t len;
    char *sub;

    len = ft_strlen(str);
    if (len >= 2)
    {
        if ((str[0] == '\'' && str[len - 1] == '\'')
            || (str[0] == '"' && str[len - 1] == '"'))
        {
            sub = ft_substr(str, 1, len - 2);
            return sub;
        }
    }
    return ft_strdup(str);
}


// static int	check_heredoc_token_error(t_token **tokens, t_command *cmd)
// {
// 	if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
// 	{
// 		ft_putstr_fd("Syntax error: missing delimiter for heredoc\n", 2);
// 		cmd->redir_error_code = 1;
// 		return (1);
// 	}
// 	return (0);
// }

static int check_heredoc_token_error(t_token **tokens, t_command *cmd)
{
    if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
    {
        ft_putstr_fd("Syntax error: missing delimiter for heredoc\n", 2);
        cmd->redir_error_code = 1;
        return (1);
    }
    return (0);
}

// void	handle_heredoc_token(t_command *cmd, t_token **tokens, t_shell *shell)
// {
// 	char			*delimiter;
// 	int				is_quoted;
// 	char			*clean;
// 	char			*tmp;

// 	*tokens = (*tokens)->next;
// 	if (check_heredoc_token_error(tokens, cmd))
// 		return ;
// 	delimiter = (*tokens)->value;
// 	is_quoted = ((*tokens)->has_single_quote || (*tokens)->has_double_quote);
// 	clean = remove_surrounding_quotes_if_any(delimiter);
// 	tmp = handle_heredoc(clean, is_quoted, shell);
// 	free(clean);
// 	if (!tmp)
// 	{
// 		cmd->redir_error_code = 1;
// 		if (g_last_signal == SIGINT)
// 		{
// 			*tokens = NULL;
// 			return ;
// 		}
// 		*tokens = (*tokens)->next;
// 		return ;
// 	}
// 	free(cmd->infile);
// 	cmd->infile = tmp;
// 	*tokens     = (*tokens)->next;
// }

void handle_heredoc_token(t_command *cmd,
                          t_token   **tokens,
                          t_shell   *shell)
{
    char *delimiter;
    int   is_quoted;
    char *clean;
    char *tmp;

    *tokens = (*tokens)->next;
    if (check_heredoc_token_error(tokens, cmd))
        return;
    delimiter = (*tokens)->value;
    is_quoted = ((*tokens)->has_single_quote
              || (*tokens)->has_double_quote);
    clean = remove_surrounding_quotes_if_any(delimiter);
    tmp = handle_heredoc(clean, is_quoted, shell);
    free(clean);
    if (!tmp)
    {
        cmd->redir_error_code = 1;
        if (shell->heredoc_interrupted)
            return;
        *tokens = (*tokens)->next;
        return;
    }
    free(cmd->infile);
    cmd->infile = tmp;
    *tokens     = (*tokens)->next;
}

void	handle_redir_out_or_append(t_command *cmd, t_token **tokens)
{
	int	is_append;
	int	flags;
	int	out_fd;

	is_append = ((*tokens)->type == TOKEN_APPEND);
	*tokens = (*tokens)->next;
	flags = O_WRONLY | O_CREAT;
	if (is_append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	out_fd = open((*tokens)->value, flags, 0644);
	if (out_fd < 0)
	{
		ft_putstr_fd((*tokens)->value, 2);
		cmd->redir_error_code = 1;
		while (*tokens && (*tokens)->type != TOKEN_PIPE)
			*tokens = (*tokens)->next;
		return ;
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
