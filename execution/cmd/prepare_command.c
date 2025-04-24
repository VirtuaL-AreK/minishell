/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 09:40:08 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/24 11:32:26 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	count_command_arguments(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (tokens->type == TOKEN_REDIR_IN || tokens->type == TOKEN_REDIR_OUT
			|| tokens->type == TOKEN_APPEND || tokens->type == TOKEN_HEREDOC)
		{
			tokens = tokens->next;
			if (tokens)
				tokens = tokens->next;
		}
		else if (tokens->type == TOKEN_WORD)
		{
			if (tokens->value[0] != '\0' || tokens->has_single_quote
				|| tokens->has_double_quote)
				count++;
			tokens = tokens->next;
		}
		else
			tokens = tokens->next;
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

void	handle_word(t_command *cmd, t_token **tokens, int *arg_count)
{
	if ((*tokens)->value[0] == '\0' && !(*tokens)->has_single_quote
		&& !(*tokens)->has_double_quote)
	{
		*tokens = (*tokens)->next;
		return ;
	}
	cmd->args[(*arg_count)++] = ft_strdup((*tokens)->value);
	*tokens = (*tokens)->next;
}
