#include "minishell.h"

static int is_special_char(int c)
{
    return (c == '|' || c == '<' || c == '>');
}

static void skip_spaces(const char *line, int *i)
{
    while (line[*i] && isspace((unsigned char)line[*i]))
        (*i)++;
}

static void add_strlist(t_strlist **head, const char *value, int has_sq, int has_dq, int should_expand)
{
    t_strlist *node = malloc(sizeof(*node));
    node->str = strdup(value);
    node->has_single_quote = has_sq;
    node->has_double_quote = has_dq;
    node->should_expand = should_expand;
    node->next = NULL;

    if (!*head)
        *head = node;
    else {
        t_strlist *tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = node;
    }
}



static char *parse_one_token_merge_quotes(const char *line, int *i,
	int *has_sq, int *has_dq,
	int *should_expand)
{
	char buffer[4096];
	int  len = 0;

	while (line[*i] && !isspace((unsigned char)line[*i]) && !is_special_char(line[*i]))
	{
		if (line[*i] == '\'')
		{
			buffer[len++] = line[*i];
			(*i)++;

			while (line[*i] && line[*i] != '\'')
			{
				buffer[len++] = line[*i];
				(*i)++;
			}
			if (line[*i] == '\'')
			{
				buffer[len++] = line[*i];
				(*i)++;
			}
		}
		else if (line[*i] == '"')
		{
			buffer[len++] = line[*i];
			(*i)++;
			while (line[*i] && line[*i] != '"')
			{
				buffer[len++] = line[*i];
				(*i)++;
			}
			if (line[*i] == '"')
			{
				buffer[len++] = line[*i];
				(*i)++;
			}
		}
		else
		{
			buffer[len++] = line[*i];
			(*i)++;
		}
	}

	buffer[len] = '\0';

	if (strchr(buffer, '\''))
	*has_sq = 1;
	if (strchr(buffer, '"'))
	*has_dq = 1;

	*should_expand = 1;

	return ft_strdup(buffer);
}




static t_strlist *bash_tokenize(const char *line)
{
    t_strlist *result = NULL;
    int i = 0;

    while (line[i])
    {
        skip_spaces(line, &i);
        if (!line[i])
            break;

        if (is_special_char(line[i]))
        {
            if (line[i] == '<' && line[i+1] == '<')
            {
                add_strlist(&result, "<<", 0, 0, 0);
                i += 2;
            }
            else if (line[i] == '>' && line[i+1] == '>')
            {
                add_strlist(&result, ">>", 0, 0, 0);
                i += 2;
            }
            else
            {
                char tmp[2] = { line[i], '\0' };
                add_strlist(&result, tmp, 0, 0, 0);
                i++;
            }
        }
        else
        {
            int has_sq = 0;
            int has_dq = 0;
            int should_expand = 0;
            char *tok = parse_one_token_merge_quotes(line, &i, &has_sq, &has_dq, &should_expand);
            if (tok && *tok)
                add_strlist(&result, tok, has_sq, has_dq, should_expand);
            free(tok);
        }
    }
    return result;
}


static t_token *build_tokens_from_list(t_strlist *lst)
{
    t_token *head = NULL;
    t_token *tail = NULL;

    while (lst)
    {
        t_token *node = malloc(sizeof(*node));
        node->value = strdup(lst->str);
        node->type  = get_token_type(node->value);

        node->has_single_quote = lst->has_single_quote;
        node->has_double_quote = lst->has_double_quote;
        node->should_expand = lst->should_expand;  // nouveau flag

        node->next = NULL;

        if (!head)
            head = node;
        else
            tail->next = node;
        tail = node;
        lst = lst->next;
    }
    return head;
}


t_token *bash_tokenize_to_tokens(const char *input)
{
    t_strlist *strs = bash_tokenize(input);        // on récupère la liste
    t_token   *tokens = build_tokens_from_list(strs); // on convertit en t_token

    while (strs)
    {
        t_strlist *tmp = strs;
        strs = strs->next;
        free(tmp->str);
        free(tmp);
    }
    return tokens;
}
