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

#include "../minishell.h"

int	process_single_quote(t_prs_ctx *ctx)
{
	if (append_char(ctx->buf, ctx->len, ctx->cap, ctx->line[*ctx->i]) < 0)
		return (-1);
	(*ctx->i)++;
	while (ctx->line[*ctx->i] && ctx->line[*ctx->i] != '\'')
	{
		if (append_char(ctx->buf, ctx->len, ctx->cap, ctx->line[*ctx->i]) < 0)
			return (-1);
		(*ctx->i)++;
	}
	if (ctx->line[*ctx->i] == '\'')
	{
		if (append_char(ctx->buf, ctx->len, ctx->cap, ctx->line[*ctx->i]) < 0)
			return (-1);
		(*ctx->i)++;
	}
	return (0);
}

t_token_type	get_token_type(char *str)
{
	if (ft_strcmp(str, "|") == 0)
		return (TOKEN_PIPE);
	if (ft_strcmp(str, "<") == 0)
		return (TOKEN_REDIR_IN);
	if (ft_strcmp(str, ">") == 0)
		return (TOKEN_REDIR_OUT);
	if (ft_strcmp(str, ">>") == 0)
		return (TOKEN_APPEND);
	if (ft_strcmp(str, "<<") == 0)
		return (TOKEN_HEREDOC);
	if (ft_strcmp(str, " ") == 0)
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
