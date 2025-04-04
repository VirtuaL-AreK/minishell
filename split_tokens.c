#include "minishell.h"

typedef struct s_token_flags {
    int has_sq;
    int has_dq;
    int should_expand;
} t_token_flags;

static int is_special_char(int c)
{
    return (c == '|' || c == '<' || c == '>');
}

static void skip_spaces(const char *line, int *i)
{
    while (line[*i] && isspace((unsigned char)line[*i]))
        (*i)++;
}

static void add_strlist(t_strlist **head, const char *value, t_token_flags flags)
{
    t_strlist *node;
    t_strlist *tmp;

    node = malloc(sizeof(*node));
    node->str = strdup(value);
    node->has_single_quote = flags.has_sq;
    node->has_double_quote = flags.has_dq;
    node->should_expand = flags.should_expand;
    node->next = NULL;
    if (!*head)
    {
        *head = node;
    }
    else
    {
        tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = node;
    }
}

static void process_single_quote(const char *line, int *i, char *buffer, int *len)
{
    buffer[*len] = line[*i];
    *len = *len + 1;
    *i = *i + 1;
    while (line[*i] && line[*i] != '\'')
    {
        buffer[*len] = line[*i];
        *len = *len + 1;
        *i = *i + 1;
    }
    if (line[*i] == '\'')
    {
        buffer[*len] = line[*i];
        *len = *len + 1;
        *i = *i + 1;
    }
}

static void process_double_quote(const char *line, int *i, char *buffer, int *len)
{
    buffer[*len] = line[*i];
    *len = *len + 1;
    *i = *i + 1;
    while (line[*i] && line[*i] != '"')
    {
        buffer[*len] = line[*i];
        *len = *len + 1;
        *i = *i + 1;
    }
    if (line[*i] == '"')
    {
        buffer[*len] = line[*i];
        *len = *len + 1;
        *i = *i + 1;
    }
}

static void process_unquoted_char(const char *line, int *i, char *buffer, int *len)
{
    buffer[*len] = line[*i];
    *len = *len + 1;
    *i = *i + 1;
}

static char *parse_one_token_merge_quotes(const char *line, int *i, t_token_flags *flags)
{
    char buffer[4096];
    int len;

    len = 0;
    while (line[*i] && !isspace((unsigned char)line[*i]) && 
           !is_special_char(line[*i]))
    {
        if (line[*i] == '\'')
            process_single_quote(line, i, buffer, &len);
        else if (line[*i] == '"')
            process_double_quote(line, i, buffer, &len);
        else
            process_unquoted_char(line, i, buffer, &len);
    }
    buffer[len] = '\0';
    if (strchr(buffer, '\''))
        flags->has_sq = 1;
    if (strchr(buffer, '"'))
        flags->has_dq = 1;
    flags->should_expand = 1;
    return ft_strdup(buffer);
}

static void process_special_char_token(const char *line, int *i, t_strlist **result)
{
    t_token_flags flags;

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
        char tmp[2];

        tmp[0] = line[*i];
        tmp[1] = '\0';
        add_strlist(result, tmp, flags);
        *i = *i + 1;
    }
}

static void process_normal_token(const char *line, int *i, t_strlist **result)
{
    t_token_flags flags;
    char *tok;

    flags.has_sq = 0;
    flags.has_dq = 0;
    flags.should_expand = 0;
    tok = parse_one_token_merge_quotes(line, i, &flags);
    if (tok && *tok)
        add_strlist(result, tok, flags);
    free(tok);
}


static t_strlist *bash_tokenize(const char *line)
{
    t_strlist *result;
    int i;

    result = NULL;
    i = 0;
    while (line[i])
    {
        skip_spaces(line, &i);
        if (!line[i])
            break;
        if (is_special_char(line[i]))
            process_special_char_token(line, &i, &result);
        else
            process_normal_token(line, &i, &result);
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
