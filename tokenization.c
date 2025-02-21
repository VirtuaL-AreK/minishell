#include "minishell.h"

t_token_type get_token_type(char *str)
{
    if (strcmp(str, "|") == 0)
        return TOKEN_PIPE;
    if (strcmp(str, "<") == 0)
        return TOKEN_REDIR_IN;
    if (strcmp(str, ">") == 0)
        return TOKEN_REDIR_OUT;
    if (strcmp(str, ">>") == 0)
        return TOKEN_APPEND;
    if (strcmp(str, "<<") == 0)
        return TOKEN_HEREDOC;
    if (strcmp(str, " ") == 0)
        return TOKEN_SPACE;
    return TOKEN_WORD;
}

t_token *create_token(char *value)
{
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token)
        return NULL;
    
    new_token->value = ft_strdup(value);
    new_token->type = get_token_type(value);
    new_token->next = NULL;
    
    return new_token;
}

void append_token(t_token **head, char *value)
{
    t_token *new_token = create_token(value);
    if (!new_token)
        return;
    
    if (!*head)
    {
        *head = new_token;
        return;
    }
    t_token *temp = *head;
    while (temp->next)
        temp = temp->next;
    temp->next = new_token;
}

void print_tokens(t_token *head)
{
    t_token *temp = head;
    while (temp)
    {
        printf("Token: %-10s Type: %d\n", temp->value, temp->type);
        temp = temp->next;
    }
}

void free_tokens(t_token *head)
{
    t_token *temp;
    while (head)
    {
        temp = head;
        head = head->next;
        free(temp->value);
        free(temp);
    }
}

void tokenization(char **args)
{
    int i;
    t_token *token_list;
    
    token_list = NULL;
    i = 0;
    while (args[i] != NULL)
    {
        append_token(&token_list, args[i]);
        i++;
    }
    print_tokens(token_list);
}