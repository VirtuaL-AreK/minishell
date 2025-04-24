/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 10:56:27 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/24 14:46:43 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_word(const char *s, t_shell *shell)
{
    t_expand_state  st;
    t_exp_ctx       ctx;
    int             ret;

    if (init_expand_state(&st, 64) < 0)
        return (NULL);
    ctx.s = s;
    ctx.idx = 0;
    ctx.in_sq = 0;
    ctx.in_dq = 0;
    ctx.st = &st;
    ctx.shell = shell;
    while (ctx.s[ctx.idx])
    {
        if (toggle_quote(&ctx))
            continue;
        if ((ret = handle_dollar(&ctx)) < 0)
            return (free_and_null(st.buffer));
        if (ret)
            continue;
        if (handle_escape(&ctx))
            continue;
        if (!ctx.in_sq && !ctx.in_dq && handle_tilde(&ctx))
            continue;
        expand_add_char(&st, ctx.s[ctx.idx++]);
    }
    expand_add_char(&st, '\0');
    return (st.buffer);
}

void expand_tokens(t_token *tokens, t_shell *shell)
{
    t_token *cur = tokens;
    while (cur)
    {
        if (cur->type == TOKEN_WORD)
        {
            char *new = expand_word(cur->value, shell);
            free(cur->value);
            cur->value = new;
        }
        cur = cur->next;
    }
}
