/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:30:47 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/05 11:57:25 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	process_normal_token(const char *line, int *i, t_strlist **result)
{
	t_token_flags	flags;
	char			*tok;

	flags.has_sq = 0;
	flags.has_dq = 0;
	flags.should_expand = 0;
	tok = parse_one_token_merge_quotes(line, i, &flags);
	if (tok && *tok)
		add_strlist(result, tok, flags);
	free(tok);
}

static t_strlist	*bash_tokenize(const char *line)
{
	t_strlist	*result;
	int			i;

	result = NULL;
	i = 0;
	while (line[i])
	{
		skip_spaces(line, &i);
		if (!line[i])
			break ;
		if (is_special_char(line[i]))
			process_special_char_token(line, &i, &result);
		else
			process_normal_token(line, &i, &result);
	}
	return (result);
}

static t_token	*build_tokens_from_list(t_strlist *lst)
{
	t_token	*head;
	t_token	*tail;
	t_token	*node;

	head = NULL;
	tail = NULL;
	while (lst)
	{
		node = malloc(sizeof(*node));
		node->value = strdup(lst->str);
		node->type = get_token_type(node->value);
		node->has_single_quote = lst->has_single_quote;
		node->has_double_quote = lst->has_double_quote;
		node->should_expand = lst->should_expand;
		node->next = NULL;
		if (!head)
			head = node;
		else
			tail->next = node;
		tail = node;
		lst = lst->next;
	}
	return (head);
}

t_token	*bash_tokenize_to_tokens(const char *input)
{
	t_strlist	*tmp;
	t_strlist	*strs;
	t_token		*tokens;

	strs = bash_tokenize(input);
	tokens = build_tokens_from_list(strs);
	while (strs)
	{
		tmp = strs;
		strs = strs->next;
		free(tmp->str);
		free(tmp);
	}
	return (tokens);
}
