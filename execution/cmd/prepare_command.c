/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 09:40:08 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/06 09:57:09 by aanmazir         ###   ########.fr       */
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

char	*handle_heredoc(const char *delimiter)
{
	int		fd;
	char	*line;
	char	*template;

	template = ft_strdup("/tmp/minishell_heredoc_XXXXXX");
	fd = mkstemp(template);
	if (fd < 0)
		return (ft_putstr_fd("mkstemp", 2), NULL);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	return (strdup(template));
}

void	handle_word(t_command *cmd, t_token **tokens, int *arg_count)
{
	cmd->args[(*arg_count)++] = ft_strdup((*tokens)->value);
	*tokens = (*tokens)->next;
}
