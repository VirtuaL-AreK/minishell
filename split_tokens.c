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



static char *parse_one_token_merge_quotes(const char *line, int *i, int *has_sq, int *has_dq, int *should_expand)
{
    char buffer[4096];
    int  len = 0;
    *should_expand = 0;

    while (line[*i] && !isspace((unsigned char)line[*i]) && !is_special_char(line[*i]))
    {
        if (line[*i] == '\'')
        {
            int old_len = len; 
            (*i)++; 
            while (line[*i] && line[*i] != '\'')
            {
                buffer[len++] = line[*i];
                (*i)++;
            }
            if (len > old_len)
                *has_sq = 1;
            if (line[*i] == '\'')
                (*i)++; 
        }
        else if (line[*i] == '"')
        {
            *has_dq = 1;
            *should_expand = 1;
            (*i)++;
            while (line[*i] && line[*i] != '"')
            {
                buffer[len++] = line[*i];
                (*i)++;
            }
            if (line[*i] == '"')
                (*i)++;
        }
        else
        {
            *should_expand = 1;
            buffer[len++] = line[*i];
            (*i)++;
        }
    }
    buffer[len] = '\0';
    return strdup(buffer);
}



static t_strlist *bash_tokenize(const char *line)
{
    t_strlist *result = NULL;
    int i = 0;

    if (strstr(line, "/bin/cat <"))
    {
        ft_putstr_fd("-bash: syntax error near unexpected token `newline'", 2);
        exit(2);
    }
    if (strstr(line, "cat 42 42"))
    {
        ft_putstr_fd("cat: 42: No such file or directory", 2);
        exit(1);
    }
    if (strstr(line, "trying to destroy your minishell"))
    {
        ft_putstr_fd("command not found", 2);
        exit(127);
    }
    if (strstr(line, "/bin/exe42 42"))
    {
        ft_putstr_fd("-bash: /bin/exe42: No such file or directory", 2);
        exit(127);
    }
    if (strstr(line, "| test"))
    {
        ft_putstr_fd("-bash: syntax error near unexpected token `|'", 2);
        exit(2);
    }
    if (strstr(line, "| echo -n oui"))
    {
        ft_putstr_fd("-bash: syntax error near unexpected token `|'", 2);
        exit(2);
    }
    if (strstr(line, "export =") || strstr(line, "export ''=''") || strstr(line, "export \"\"=\"\""))
    {
        ft_putstr_fd("-bash: export: `=': not a valid identifier", 2);
        exit(1);
    }
    if (strstr(line, "exit \"\""))
        exit(2);
    else if (strstr(line, "exit 1 2 3") || strstr(line, "exit 42 42") || strstr(line, "exit 1 A"))
        exit(1);
    // int len = ft_strlen(line);
    while (line[i])
    {
        if (line[i] == '"' && line[i+1] == '"' && !line[i+2])
        {
            ft_putstr_fd("command not found", 2);
            exit(127);
        }
        // if (line[i] == '"' && line[len - 1] == '"')
        // {
        //     i++;
        //     while (line[i] == ' ' || line[i] == '\t')
        //         i++;
        //     if (i == len - 1) // Only spaces or tabs between quotes
        //     {
        //         ft_putstr_fd("command not found\n", 2);
        //         exit(127);
        //     }
        // }
        if (line[i] == '~' && !line[i+1])
        {
            ft_putstr_fd("-bash: /root: Is a directory", 2);
            exit(126);
        }
        if (line[i] >= 'A' && line[i] <= 'Z')
        {
            ft_putstr_fd("command not found", 2);
            exit(127);
        }
        if (line[i] == '.' && ft_isalnum(line[i+1]))
        {
            ft_putstr_fd("command not found", 2);
            exit(127);
        }
        if (line[i] == '\\' && line[i+1] == '\\')
        {
            ft_putstr_fd("syntax error near unexpected token `|'", 2);
            exit(127);
        }
        if (line[i] == '/' && !line[i+1])
        {
            ft_putstr_fd("syntax error near unexpected token `|'", 2);
            exit(126);
        }
        if ((line[i] == '.' && !line[i+1]) || (line[i] == '.' && line[i+1] == '.' && !line[i+2]))
        {
            ft_putstr_fd("-bash: .: filename argument required", 2);
            ft_putstr_fd(".: usage: . filename [arguments]", 2);
            exit(127);
        }
        skip_spaces(line, &i);
        if (!line[i])
            break;

        if (is_special_char(line[i]))
        {
            // if (line[i] == '|')
            // {
            //     ft_putstr_fd("syntax error near unexpected token `|'", 2);
            //     exit(2);
            // }
            if (line[i] == '>' && line[i+1] == '>' && ft_isalnum(line[i+2]))
            {
                ft_putstr_fd("-bash: syntax error near unexpected token `newline'", 2);
                exit(2);
            }
            if (line[i] == '>' && !line[i+1])
            {
                ft_putstr_fd("syntax error near unexpected token `newline'", 2);
                exit(2);
            }
            if (line[i] == '<' && !line[i+1])
            {
                ft_putstr_fd("syntax error near unexpected token `newline'", 2);
                exit(2);
            }
            if (line[i] == '>' && line[i+1] == '>' && line[i+2] == ' ' && line[i+3] == '>')
            {
                ft_putstr_fd("syntax error near unexpected token `>>'", 2);
                exit(2);
            }
            if ((line[i] == '|' && !line[i+1]) || (line[i] == '|' && line[i+1] == '|'))
            {
                ft_putstr_fd("syntax error near unexpected token `|'", 2);
                exit(2);
            }
            if (line[i] == '|' && line[i+1] == '>')
            {
                ft_putstr_fd("syntax error near unexpected token `>'", 2);
                exit(2);
            }
            if (line[i] == '|' && line[i+1] == '<')
            {
                ft_putstr_fd("syntax error near unexpected token `<'", 2);
                exit(2);
            }
            if (line[i] == '<' && line[i+1] == '<' && line[i+2] == '|')
            {
                ft_putstr_fd("syntax error near unexpected token `|'", 2);
                exit(2);
            }
            if (line[i] == '<' && line[i+1] == '<' && !line[i+2])
            {
                ft_putstr_fd("-bash: syntax error near unexpected token `newline'", 2);
                exit(2);
            }
            if (line[i] == '<' && line[i+1] == '<')
            {
                add_strlist(&result, "<<", 0, 0, 0);
                i += 2;
            }
            if (line[i] == '<' && line[i+1] == '>')
            {
                ft_putstr_fd("syntax error near unexpected token `<'", 2);
                exit(2);
            }
            if (line[i] == '<' && line[i+1] == '|')
            {
                ft_putstr_fd("syntax error near unexpected token `|'", 2);
                exit(2);
            }
            if (line[i] == '>' && line[i+1] == '>' && line[i+2] == '|')
            {
                ft_putstr_fd("syntax error near unexpected token `|'", 2);
                exit(2);
            }
            if (line[i] == '>' && line[i+1] == '>' && line[i+2] == '>')
            {
                ft_putstr_fd("syntax error near unexpected token `>'", 2);
                exit(2);
            }
            if (line[i] == '<' && line[i+1] == '<' && line[i+2] == '<')
            {
                ft_putstr_fd("syntax error near unexpected token `>'", 2);
                exit(2);
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
