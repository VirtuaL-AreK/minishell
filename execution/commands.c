/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 20:21:48 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/18 11:35:32 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_command *command_parser(t_token *tokens, t_shell *shell)
{
    t_command *head = NULL;
    t_command *last = NULL;
    t_command *cmd;

    while (tokens)
    {
        cmd = new_command(tokens);
        fill_command(cmd, &tokens, shell);    // on passe shell ici
        fix_empty_first_arg(cmd);
        if (!head)
            head = cmd;
        else
            last->next = cmd;
        last = cmd;

        if (tokens && tokens->type == TOKEN_PIPE)
            tokens = tokens->next;
    }

    return head;
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
