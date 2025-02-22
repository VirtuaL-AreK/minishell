#include "minishell.h"

void print_command(t_command *commands)
{
	t_command *cur;

	cur = commands;
	int i = 0;
	while(cur)
	{
		printf(YELLOW "command : " RESET);
		while(cur->args[i])
		{
			printf(YELLOW "%s " RESET, cur->args[i]);
			i++;
		}
		i = 0;
		printf("; infile : %s |  outfile : %s  | append  : %d   | nb_arg : %i\n", cur->infile, cur->outfile, cur->append, cur->nb_arg);
		cur = cur->next;
	}
}

t_command *new_command(t_token *tokens)
{
	t_command *cmd;
	t_token *tmp;
	int i;

	cmd = malloc(sizeof(t_command));

	i = 0;
	tmp = tokens;
	while(tmp && tmp->type == 0)
	{
		tmp = tmp->next;
		i++;
	}
	cmd->nb_arg = i;
	cmd->args = malloc(sizeof(char *) * (cmd->nb_arg + 1));
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->next = NULL;

	return(cmd);
}

void fill_command(t_command *cmd, t_token **tokens)
{
    int arg_count;

    arg_count = 0;
    while (*tokens && (*tokens)->type != 1)
    {
        if ((*tokens)->type == 0)
            cmd->args[arg_count++] = ft_strdup((*tokens)->value);
        else if ((*tokens)->type == 2 && (*tokens)->next)
        {
            *tokens = (*tokens)->next;
            cmd->infile = ft_strdup((*tokens)->value);
        }
        else if ((*tokens)->type == 3 || (*tokens)->type == 4)
        {
            if ((*tokens)->type == 4)
                cmd->append = 1;
            if ((*tokens)->next)
            {
                *tokens = (*tokens)->next;
                cmd->outfile = ft_strdup((*tokens)->value);
            }
        }
        *tokens = (*tokens)->next;
    }
    cmd->args[arg_count] = NULL;
}

t_command *command_parser(t_token *tokens)
{
    t_command *head;
	t_command *last;
	t_command *cmd;

	head = NULL;
	last = NULL;
    while (tokens) 
	{
		cmd = new_command(tokens);
		fill_command(cmd, &tokens);
        if (!head)
            head = cmd;
        else
            last->next = cmd;
        last = cmd;
        if (tokens && tokens->type == 1)
            tokens = tokens->next;
    }
    return head;
}

void free_commands(t_command *cmd)
{
    t_command *tmp;
    int i;

    while (cmd)
    {
        tmp = cmd->next;
        
        if (cmd->args)
        {
            i = 0;
            while (cmd->args[i])
                free(cmd->args[i++]);
            free(cmd->args);
        }
        if (cmd->infile)
            free(cmd->infile);
        if (cmd->outfile)
            free(cmd->outfile);

        free(cmd);
        cmd = tmp;
    }
}
