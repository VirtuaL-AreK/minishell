/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 10:56:27 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/23 17:28:35 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_expand_state(t_expand_state *st, int init_cap)
{
	st->buffer = (char *)malloc(init_cap);
	if (!st->buffer)
		return (-1);
	st->capacity = init_cap;
	st->idx = 0;
	st->in_sq = 0;
	st->in_dq = 0;
	return (0);
}

int	expand_buffer_if_needed(t_expand_state *st, int needed)
{
	char	*new_buf;
	int		new_cap;

	if (st->idx + needed < st->capacity)
		return (0);
	new_cap = st->capacity;
	while (new_cap < st->idx + needed)
		new_cap *= 2;
	new_buf = (char *)malloc(new_cap);
	if (!new_buf)
		return (-1);
	memcpy(new_buf, st->buffer, st->idx);
	free(st->buffer);
	st->buffer = new_buf;
	st->capacity = new_cap;
	return (0);
}


int	expand_add_char(t_expand_state *st, char c)
{
	if (expand_buffer_if_needed(st, 1 + 1) < 0)
		return (-1);
	st->buffer[st->idx] = c;
	st->idx++;
	return (0);
}

int	expand_add_string(t_expand_state *st, const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (expand_add_char(st, s[i]) < 0)
			return (-1);
		i++;
	}
	return (0);
}

int	process_dollar_branch(const char *str, int i, t_expand_state *state, t_shell *shell)
{
	if (str[i + 1] == '\'' || str[i + 1] == '"')
		handle_dollar_quoted(str, &i, state, shell);
	else
		handle_variable(str, &i, state, shell);
	return (i);
}

int	process_backslash_branch(const char *str, int i, t_expand_state *state)
{
	i = i + 1;
	if (str[i])
	{
		state->buffer[state->idx] = str[i];
		state->idx = state->idx + 1;
		return (i + 1);
	}
	return (i);
}

static int	handle_dollar_expand(const char *str,
                                  int           *i,
                                  t_expand_state *st,
                                  t_shell        *shell)
{
    if (handle_variable(str, i, st, shell) < 0)
        return (-1);
    return (*i);
}

static int	handle_quote_toggle(const char *str,
                                int            i,
                                t_expand_state *st)
{
    if (str[i] == '\'' && !st->in_dq)
    {
        st->in_sq = !st->in_sq;
        return (i + 1);
    }
    if (str[i] == '"' && !st->in_sq)
    {
        st->in_dq = !st->in_dq;
        return (i + 1);
    }
    return (-1);
}

static int	handle_backslash_expand(const char *str,
                                    int            i,
                                    t_expand_state *st)
{
    i++;
    if (str[i])
    {
        if (expand_add_char(st, str[i]) < 0)
            return (-1);
        i++;
    }
    return (i);
}

static int	handle_regular_char(const char *str,
                                int            i,
                                t_expand_state *st)
{
    if (expand_add_char(st, str[i]) < 0)
        return (-1);
    return (i + 1);
}

int	process_char_in_expand(const char    *str,
                          int             i,
                          t_expand_state *st,
                          t_shell        *shell)
{
    int ret;

    if (!st->in_sq && str[i] == '$')
        return handle_dollar_expand(str, &i, st, shell);
    ret = handle_quote_toggle(str, i, st);
    if (ret >= 0)
        return ret;
    if (str[i] == '\\')
        return handle_backslash_expand(str, i, st);
    return handle_regular_char(str, i, st);
}

char	*expand_string(const char *str, t_shell *shell)
{
	t_expand_state	st;
	int				i;
	int				ret;

	(void)shell;
	if (!str)
		return (NULL);
	if (init_expand_state(&st, 64) < 0)
		return (NULL);
	i = 0;
	while (str[i])
	{
		ret = process_char_in_expand(str, i, &st, shell); 
		if (ret < 0)
		{
			free(st.buffer);
			return (NULL);
		}
		i = ret; 
	}
	if (expand_add_char(&st, '\0') < 0)
	{
		return (free(st.buffer), NULL);
	}
	return (st.buffer);
}

char *expand_tilde(const char *value, t_shell *shell)
{
	char *home;
    if (!value || value[0] != '~')
        return ft_strdup(value);

    if (value[1] == '\0' || value[1] == '/')
    {
        home = get_local_env_value("HOME", shell);
        if (!home)
            return ft_strdup(value);

        char *result;
        if (value[1] == '/')
        {
            result = ft_strjoin(home, value + 1);
        }
        else
        {
            result = home;
        }
        return result;
    }
    return ft_strdup(value);
}

char *remove_quotes(const char *s)
{
    t_expand_state st;
    int in_sq = 0, in_dq = 0, i = 0;

    if (init_expand_state(&st, 64) < 0)
        return NULL;

    while (s[i])
    {
        if (s[i] == '\'' && !in_dq) 
            in_sq = !in_sq;
        else if (s[i] == '"' && !in_sq) 
            in_dq = !in_dq;
        else           
            expand_add_char(&st, s[i]);
        i++;
    }
    expand_add_char(&st, '\0');
    return st.buffer;
}

char *expand_word(const char *s, t_shell *shell)
{
    t_expand_state st;
    int in_sq = 0, in_dq = 0, i = 0;

    if (init_expand_state(&st, 64) < 0)
        return NULL;

    while (s[i])
    {
        if (s[i] == '\'' && !in_dq)
        {
            in_sq = !in_sq;
            i++;
        }
        else if (s[i] == '"' && !in_sq)
        {
            in_dq = !in_dq;
            i++;
        }
		else if (!in_sq && !in_dq && s[i] == '$'
			&& (s[i + 1] == '\'' || s[i + 1] == '"'))
	  {
		  if (handle_dollar_quoted(s, &i, &st, shell) < 0)
		  {
			  free(st.buffer);
			  return NULL;
		  }
	  }
	  else if (!in_sq && s[i] == '$')
	  {
		  if (handle_variable(s, &i, &st, shell) < 0)
		  {
			  free(st.buffer);
			  return NULL;
		  }
	  }	  
        else if (in_dq && s[i] == '\\'
                 && (s[i+1] == '"' || s[i+1] == '\\'
                     || s[i+1] == '$'  || s[i+1] == '`'))
        {
            i++;
            expand_add_char(&st, s[i]);
            i++;
        }
        else if (!in_sq && !in_dq && s[i] == '\\')
        {
            i++;
            if (s[i])
                expand_add_char(&st, s[i]);
            i++;
        }
        else if (!in_sq && !in_dq && i == 0 && s[i] == '~'
                 && (s[i+1] == '\0' || s[i+1] == '/'))
        {
            char *home = get_local_env_value("HOME", shell);
            expand_add_string(&st, home);
            free(home);
            i++;
        }
        else
        {
            expand_add_char(&st, s[i]);
            i++;
        }
    }

    expand_add_char(&st, '\0');
    return st.buffer;
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


