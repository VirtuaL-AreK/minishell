#include "minishell.h"

typedef struct s_expand_state {
	char buffer[4096];
	int idx;
	int in_sq;
	int in_dq;
} t_expand_state;

static char *get_local_env_value(const char *var, t_shell *shell)
{
	int i;
	int len;
	char *ret;

	i = 0;
	len = ft_strlen(var);
	while (shell->env && shell->env[i])
	{
		if (ft_strncmp(shell->env[i], var, len) == 0 &&
			shell->env[i][len] == '=')
		{
			ret = ft_strdup(shell->env[i] + len + 1);
			return ret;
		}
		i = i + 1;
	}
	return ft_strdup("");
}

static char process_escape_char(const char *s, int *i, char quote)
{
	char res;

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
	return res;
}

static void process_backslash(const char *s, int *i, int *j, 
							char *result, char quote)
{
	(*i) = (*i) + 1;
	if (s[*i])
	{
		result[*j] = process_escape_char(s, i, quote);
		(*j) = (*j) + 1;
	}
}

char *process_ansi_c(const char *s)
{
	int i, j, len;
	char *result;

	i = 0;
	j = 0;
	len = ft_strlen(s);
	result = malloc(len + 1);
	if (result == NULL)
		return NULL;
	while (s[i])
	{
		if (s[i] == '\\')
			process_backslash(s, &i, &j, result, '\'');
		else
		{
			result[j] = s[i];
			i = i + 1;
			j = j + 1;
		}
	}
	result[j] = '\0';
	return result;
}

char *process_dollar_dquote(const char *s)
{
	int i, j, len;
	char *result;

	i = 0;
	j = 0;
	len = ft_strlen(s);
	result = malloc(len + 1);
	if (result == NULL)
		return NULL;
	while (s[i])
	{
		if (s[i] == '\\')
			process_backslash(s, &i, &j, result, '"');
		else
		{
			result[j] = s[i];
			i = i + 1;
			j = j + 1;
		}
	}
	result[j] = '\0';
	return result;
}

static void append_string(t_expand_state *state, const char *s)
{
	int k;

	k = 0;
	while (s[k] && state->idx < 4095)
	{
		state->buffer[state->idx] = s[k];
		state->idx = state->idx + 1;
		k = k + 1;
	}
}

static void handle_dollar_quoted(const char *s, int *i, 
								t_expand_state *state, t_shell *shell)
{
	char quote;
	int seg_start;
	int seg_len;
	char *segment;
	char *processed;

	(void)shell;
	quote = s[*i + 1];
	*i = *i + 2;
	seg_start = *i;
	while (s[*i] && s[*i] != quote)
		(*i) = (*i) + 1;
	seg_len = *i - seg_start;
	segment = strndup(s + seg_start, seg_len);
	if (quote == '\'')
		processed = process_ansi_c(segment);
	else
		processed = process_dollar_dquote(segment);
	free(segment);
	append_string(state, processed);
	free(processed);
	if (s[*i] == quote)
		(*i) = (*i) + 1;
}

static void handle_alphanum_variable(const char *s, int *i, 
									t_expand_state *state, t_shell *shell)
{
	int start;
	int var_len;
	char *var_name;
	char *val;

	start = *i;
	while (s[*i] &&
		(((s[*i] >= 'A' && s[*i] <= 'Z') ||
			(s[*i] >= 'a' && s[*i] <= 'z') ||
			(s[*i] >= '0' && s[*i] <= '9')) ||
			s[*i] == '_'))
		(*i) = (*i) + 1;
	var_len = *i - start;
	var_name = strndup(s + start, var_len);
	val = get_local_env_value(var_name, shell);
	free(var_name);
	append_string(state, val);
	free(val);
}

static void handle_variable(const char *s, int *i, 
							t_expand_state *state, t_shell *shell)
{
	char *exit_str;

	(*i) = (*i) + 1; 
	if (s[*i] == '?')
	{
		(*i) = (*i) + 1;
		exit_str = ft_itoa(shell->exit_status);
		append_string(state, exit_str);
		free(exit_str);
	}
	else if ((s[*i] >= 'A' && s[*i] <= 'Z') ||
			(s[*i] >= 'a' && s[*i] <= 'z') ||
			s[*i] == '_')
	{
		handle_alphanum_variable(s, i, state, shell);
	}
	else
	{
		state->buffer[state->idx] = '$';
		state->idx = state->idx + 1;
	}
}

static int process_dollar_branch(const char *str, int i, 
								t_expand_state *state, t_shell *shell)
{
	if (str[i + 1] == '\'' || str[i + 1] == '"')
	{
		handle_dollar_quoted(str, &i, state, shell);
	}
	else
	{
		handle_variable(str, &i, state, shell);
	}
	return i;
}

static int process_backslash_branch(const char *str, int i, 
									t_expand_state *state)
{
	i = i + 1;
	if (str[i])
	{
		state->buffer[state->idx] = str[i];
		state->idx = state->idx + 1;
		return i + 1;
	}
	return i;
}

static int process_char_in_expand(const char *str, int i, 
								t_expand_state *state, t_shell *shell)
{
	if (!state->in_sq && str[i] == '$')
		return process_dollar_branch(str, i, state, shell);
	else if (str[i] == '\'' && !state->in_dq)
	{
		state->in_sq = !state->in_sq;
		return i + 1;
	}
	else if (str[i] == '"' && !state->in_sq)
	{
		state->in_dq = !state->in_dq;
		return i + 1;
	}
	else if (str[i] == '\\')
		return process_backslash_branch(str, i, state);
	else
	{
		state->buffer[state->idx] = str[i];
		state->idx = state->idx + 1;
		return i + 1;
	}
}


char *expand_string(const char *str, t_shell *shell)
{
	t_expand_state state;
	int i;
	char *result;

	state.idx = 0;
	state.in_sq = 0;
	state.in_dq = 0;
	i = 0;
	while (str[i] && state.idx < 4095)
	{
		i = process_char_in_expand(str, i, &state, shell);
	}
	state.buffer[state.idx] = '\0';
	result = ft_strdup(state.buffer);
	return result;
}

void expand_tokens(t_token *tokens, t_shell *shell)
{
	t_token *cur;
	char *expanded;

	cur = tokens;
	while (cur)
	{
		if (cur->type == TOKEN_WORD && cur->should_expand)
		{
			expanded = expand_string(cur->value, shell);
			free(cur->value);
			cur->value = expanded;
		}
		cur = cur->next;
	}
}
