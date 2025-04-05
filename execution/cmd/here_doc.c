/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 20:28:14 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/05 20:35:25 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_redir_in(t_command *cmd, t_token **tokens)
{
	int	in_fd;

	*tokens = (*tokens)->next;
	in_fd = open((*tokens)->value, O_RDONLY);
	if (in_fd < 0)
	{
		perror((*tokens)->value);
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

void	handle_heredoc_token(t_command *cmd, t_token **tokens)
{
	char	*delimiter;
	char	*temp_file;

	*tokens = (*tokens)->next;
	if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
	{
		ft_putstr_fd("Syntax error: missing delimiter for heredoc\n", 2);
		cmd->redir_error_code = 1;
		return ;
	}
	delimiter = (*tokens)->value;
	temp_file = handle_heredoc(delimiter);
	if (!temp_file)
	{
		cmd->redir_error_code = 1;
		*tokens = (*tokens)->next;
		return ;
	}
	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = temp_file;
	*tokens = (*tokens)->next;
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
		perror((*tokens)->value);
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

void	fill_command(t_command *cmd, t_token **tokens)
{
	int	arg_count;

	arg_count = 0;
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		if ((*tokens)->type == TOKEN_WORD)
			handle_word(cmd, tokens, &arg_count);
		else if ((*tokens)->type == TOKEN_REDIR_IN && (*tokens)->next)
			handle_redir_in(cmd, tokens);
		else if ((*tokens)->type == TOKEN_HEREDOC)
			handle_heredoc_token(cmd, tokens);
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
