#include "../../minishell.h"

char *free_and_null(char *buf)
{
    free(buf);
    return (NULL);
}

int toggle_quote(t_exp_ctx *ctx)
{
    if (ctx->s[ctx->idx] == '\'' && !ctx->in_dq)
    {
        ctx->in_sq = !ctx->in_sq;
        ctx->idx++;
        return (1);
    }
    if (ctx->s[ctx->idx] == '"' && !ctx->in_sq)
    {
        ctx->in_dq = !ctx->in_dq;
        ctx->idx++;
        return (1);
    }
    return (0);
}

int handle_dollar(t_exp_ctx *ctx)
{
    if (ctx->in_sq || ctx->s[ctx->idx] != '$')
        return (0);
    if (!ctx->in_dq && (ctx->s[ctx->idx + 1] == '\'' || ctx->s[ctx->idx + 1] == '"'))
    {
        if (handle_dollar_quoted(ctx->s, &ctx->idx, ctx->st, ctx->shell) < 0)
            return (-1);
        return (1);
    }
    if (handle_variable(ctx->s, &ctx->idx, ctx->st, ctx->shell) < 0)
        return (-1);
    return (1);
}

int handle_escape(t_exp_ctx *ctx)
{
    if (ctx->in_dq && ctx->s[ctx->idx] == '\\'
        && (ctx->s[ctx->idx + 1] == '"' || ctx->s[ctx->idx + 1] == '\\'
         || ctx->s[ctx->idx + 1] == '$'  || ctx->s[ctx->idx + 1] == '`'))
    {
        ctx->idx++;
        expand_add_char(ctx->st, ctx->s[ctx->idx]);
        ctx->idx++;
        return (1);
    }
    if (!ctx->in_sq && !ctx->in_dq && ctx->s[ctx->idx] == '\\')
    {
        ctx->idx++;
        if (ctx->s[ctx->idx])
            expand_add_char(ctx->st, ctx->s[ctx->idx]);
        ctx->idx++;
        return (1);
    }
    return (0);
}

int handle_tilde(t_exp_ctx *ctx)
{
    char *home;

    if (ctx->idx == 0 && ctx->s[ctx->idx] == '~'
        && (ctx->s[ctx->idx + 1] == '\0' || ctx->s[ctx->idx + 1] == '/'))
    {
        home = get_local_env_value("HOME", ctx->shell);
        expand_add_string(ctx->st, home);
        free(home);
        ctx->idx++;
        return (1);
    }
    return (0);
}