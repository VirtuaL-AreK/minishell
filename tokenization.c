/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 10:59:54 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/05 11:07:10 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type	get_token_type(char *str)
{
	if (strcmp(str, "|") == 0)
		return (TOKEN_PIPE);
	if (strcmp(str, "<") == 0)
		return (TOKEN_REDIR_IN);
	if (strcmp(str, ">") == 0)
		return (TOKEN_REDIR_OUT);
	if (strcmp(str, ">>") == 0)
		return (TOKEN_APPEND);
	if (strcmp(str, "<<") == 0)
		return (TOKEN_HEREDOC);
	if (strcmp(str, " ") == 0)
		return (TOKEN_SPACE);
	return (TOKEN_WORD);
}

void	print_tokens(t_token *head)
{
	t_token	*temp;

	temp = head;
	while (temp)
	{
		printf("Token: %-10s Type: %d\n", temp->value, temp->type);
		temp = temp->next;
	}
}

void	free_tokens(t_token *head)
{
	t_token	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->value);
		free(temp);
	}
}
