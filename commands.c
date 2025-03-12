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

int count_command_arguments(t_token *tokens)
{
    int count = 0;
    while (tokens && tokens->type != 1)
    {
        if (tokens->type == 2 || tokens->type == 3 || tokens->type == 4)
        {
            tokens = tokens->next;
            if (tokens)
                tokens = tokens->next;
        }
        else if (tokens->type == 0)
        {
            count++;
            tokens = tokens->next;
        }
        else
        {
            tokens = tokens->next;
        }
    }
    return count;
}

t_command *new_command(t_token *tokens)
{
    t_command *cmd;
    int nb_arg;

    cmd = malloc(sizeof(t_command));
    if (!cmd)
        return (NULL);
    nb_arg = count_command_arguments(tokens);
    cmd->nb_arg = nb_arg;
    cmd->args = malloc(sizeof(char *) * (nb_arg + 1));
    if (!cmd->args)
    {
        free(cmd);
        return (NULL);
    }
    cmd->infile = NULL;
    cmd->outfile = NULL;
    cmd->append = 0;
	cmd->redir_error_code = 0;
    cmd->next = NULL;
    return (cmd);
}

void fill_command(t_command *cmd, t_token **tokens)
{
    int arg_count = 0;

    while (*tokens && (*tokens)->type != 1)
    {
        if ((*tokens)->type == 0)
        {
            cmd->args[arg_count++] = ft_strdup((*tokens)->value);
            *tokens = (*tokens)->next;
        }
        else if ((*tokens)->type == 2 && (*tokens)->next)
        {
            *tokens = (*tokens)->next;  // passer le token "<"
            int in_fd = open((*tokens)->value, O_RDONLY);
            if (in_fd < 0)
            {
                perror((*tokens)->value);
                cmd->redir_error_code = 1;
                while (*tokens && (*tokens)->type != 1)
                    *tokens = (*tokens)->next;
                break;
            }
            else
            {
                close(in_fd);
            }
            if (cmd->infile)
                free(cmd->infile);
            cmd->infile = ft_strdup((*tokens)->value);
            *tokens = (*tokens)->next;
        }
        else if (((*tokens)->type == 3 || (*tokens)->type == 4))
        {
            if (!(*tokens)->next)
            {
                ft_putstr_fd("Syntax error: missing file for redirection\n", 2);
                cmd->redir_error_code = 1;
                while (*tokens && (*tokens)->type != 1)
                    *tokens = (*tokens)->next;
                break;
            }
            if ((*tokens)->type == 4)
                cmd->append = 1;
            else
                cmd->append = 0;
            *tokens = (*tokens)->next;  // passer au token contenant le nom du fichier

            int flags = O_WRONLY | O_CREAT;
            if (cmd->append)
                flags |= O_APPEND;
            else
                flags |= O_TRUNC;
            int fd = open((*tokens)->value, flags, 0644);
            if (fd < 0)
            {
                perror((*tokens)->value);
                cmd->redir_error_code = 1;
                if (cmd->outfile)
                {
                    free(cmd->outfile);
                    cmd->outfile = NULL;
                }
                while (*tokens && (*tokens)->type != 1)
                    *tokens = (*tokens)->next;
                break;
            }
            else
            {
                close(fd);
            }
            if (cmd->outfile)
                free(cmd->outfile);
            cmd->outfile = ft_strdup((*tokens)->value);
            *tokens = (*tokens)->next;
        }
        else
        {
            *tokens = (*tokens)->next;
        }
    }
    cmd->args[arg_count] = NULL;
}


static void fix_empty_first_arg(t_command *cmd)
{
    if (!cmd->args[0])
        return;

    if (cmd->args[0][0] == '\0')
    {
        if (!cmd->args[1])
        {
            free(cmd->args[0]);
            cmd->args[0] = NULL;
            cmd->nb_arg = 0;
        }
        else
        {
            free(cmd->args[0]);
            int i = 0;
            while (cmd->args[i+1])
            {
                cmd->args[i] = cmd->args[i+1];
                i++;
            }
            cmd->args[i] = NULL;
            cmd->nb_arg--;
        }
    }
}

t_command *command_parser(t_token *tokens)
{
    t_command *head = NULL;
    t_command *last = NULL;

    while (tokens)
    {
        t_command *cmd = new_command(tokens);
        fill_command(cmd, &tokens);

        fix_empty_first_arg(cmd);

        if (!head)
            head = cmd;
        else
            last->next = cmd;
        last = cmd;

        if (tokens && tokens->type == TOKEN_PIPE)
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
