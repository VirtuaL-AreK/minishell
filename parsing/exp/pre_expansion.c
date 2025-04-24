/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:15:47 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/24 17:52:46 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	process_escape_char(const char *s, int *i, char quote)
{
	char	res;

	if (s[*i] == 'n')
		res = '\n';
	else if (s[*i] == 't')
		res = '\t';
	else if (s[*i] == '\\')
		res = '\\';
	else if (s[*i] == quote)
		res = quote;
	else
		res = s[*i];
	(*i) = (*i) + 1;
	return (res);
}

void	process_backslash(t_process_ctx *ctx, const char *s)
{
	ctx->i = ctx->i + 1;
	if (s[ctx->i])
	{
		ctx->result[ctx->j] = process_escape_char(s, &ctx->i, ctx->quote);
		ctx->j = ctx->j + 1;
	}
}

char	*process_ansi_c(const char *s)
{
	t_process_ctx	ctx;
	int				len;

	len = ft_strlen(s);
	ctx.result = malloc(len + 1);
	if (ctx.result == NULL)
		return (NULL);
	ctx.i = 0;
	ctx.j = 0;
	ctx.quote = '\'';
	while (s[ctx.i])
	{
		if (s[ctx.i] == '\\')
			process_backslash(&ctx, s);
		else
		{
			ctx.result[ctx.j] = s[ctx.i];
			ctx.i = ctx.i + 1;
			ctx.j = ctx.j + 1;
		}
	}
	ctx.result[ctx.j] = '\0';
	return (ctx.result);
}

char	*process_dollar_dquote(const char *s)
{
	t_process_ctx	ctx;
	int				len;

	len = ft_strlen(s);
	ctx.result = malloc(len + 1);
	if (ctx.result == NULL)
		return (NULL);
	ctx.i = 0;
	ctx.j = 0;
	ctx.quote = '"';
	while (s[ctx.i])
	{
		if (s[ctx.i] == '\\')
			process_backslash(&ctx, s);
		else
		{
			ctx.result[ctx.j] = s[ctx.i];
			ctx.i = ctx.i + 1;
			ctx.j = ctx.j + 1;
		}
	}
	ctx.result[ctx.j] = '\0';
	return (ctx.result);
}

int	append_string(t_expand_state *st, const char *s)
{
	if (!s)
		return (0);
	return (expand_add_string(st, s));
}
