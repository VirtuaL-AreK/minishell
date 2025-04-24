/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens_01.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:01:56 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/24 12:21:30 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// static int	accumulate_token(const char *line,
// 				int *i,
// 				char **buf,
// 				int *len,
// 				int *cap)
// {
// 	int	ret;

// 	while (line[*i] && !isspace((unsigned char)line[*i])
// 	    && !is_special_char(line[*i]))
// 	{
// 		if (line[*i] == '\'')
// 			ret = process_single_quote(line, i, buf, len, cap);
// 		else if (line[*i] == '"')
// 			ret = process_double_quote(line, i, buf, len, cap);
// 		else
// 			ret = process_unquoted_char(line, i, buf, len, cap);
// 		if (ret < 0)
// 			return (-1);
// 	}
// 	return (0);
// }

static int	accumulate_token(t_prs_ctx *ctx)
{
	int	ret;

	while (ctx->line[*ctx->i]
	    && !isspace((unsigned char)ctx->line[*ctx->i])
	    && !is_special_char(ctx->line[*ctx->i]))
	{
		if (ctx->line[*ctx->i] == '\'')
			ret = process_single_quote(ctx);
		else if (ctx->line[*ctx->i] == '\"')
			ret = process_double_quote(ctx);
		else
			ret = process_unquoted_char(ctx);
		if (ret < 0)
			return (-1);
	}
	return (0);
}

static void	set_quote_flags(t_token_flags *flags, const char *buf)
{
	if (strchr(buf, '\''))
		flags->has_sq = 1;
	if (strchr(buf, '"'))
		flags->has_dq = 1;
}

char	*parse_one_token_merge_quotes(const char *line,
			int *i,
			t_token_flags *flags)
{
	t_prs_ctx	ctx;
	char		*buf;
	int			capacity = 64;
	int			len = 0;

	buf = malloc(capacity);
	if (!buf)
		return (NULL);
	ctx.line = line;
	ctx.i    = i;
	ctx.buf  = &buf;
	ctx.len  = &len;
	ctx.cap  = &capacity;

	if (accumulate_token(&ctx) < 0)
		return (free(buf), NULL);
	if (append_char(&buf, &len, &capacity, '\0') < 0)
		return (free(buf), NULL);
	set_quote_flags(flags, buf);
	flags->should_expand = 1;
	return (buf);
}


void	process_special_char_token(const char *line, int *i, t_strlist **result)
{
	char			tmp[2];
	t_token_flags	flags;

	flags.has_sq = 0;
	flags.has_dq = 0;
	flags.should_expand = 0;
	if (line[*i] == '<' && line[*i + 1] == '<')
	{
		add_strlist(result, "<<", flags);
		*i = *i + 2;
	}
	else if (line[*i] == '>' && line[*i + 1] == '>')
	{
		add_strlist(result, ">>", flags);
		*i = *i + 2;
	}
	else
	{
		tmp[0] = line[*i];
		tmp[1] = '\0';
		add_strlist(result, tmp, flags);
		*i = *i + 1;
	}
}
