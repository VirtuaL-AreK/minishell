/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 20:21:48 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/05 20:26:01 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_command	*command_parser(t_token *tokens)
{
	t_command	*head;
	t_command	*last;
	t_command	*cmd;

	head = NULL;
	last = NULL;
	while (tokens)
	{
		cmd = new_command(tokens);
		fill_command(cmd, &tokens);
		fix_empty_first_arg(cmd);
		if (!head)
			head = cmd;
		else
			last->next = cmd;
		last = cmd;
		if (tokens && tokens->type == TOKEN_PIPE)
			tokens = tokens->next;
	}
	return (head);
}

void	free_commands(t_command *cmd)
{
	t_command	*tmp;
	int			i;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
				free(cmd->args[i++]);
			free(cmd->args);
		}
		if (cmd->infile)
			free(cmd->infile);
		if (cmd->outfile)
			free(cmd->outfile);
		free(cmd);
		cmd = tmp;
	}
}
