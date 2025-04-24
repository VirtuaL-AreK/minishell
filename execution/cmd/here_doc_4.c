/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:30:46 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/24 13:34:17 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_redir_in(t_command *cmd, t_token **tokens)
{
	int	in_fd;

	*tokens = (*tokens)->next;
	in_fd = open((*tokens)->value, O_RDONLY);
	if (in_fd < 0)
	{
		print_error((*tokens)->value);
		cmd->redir_error_code = 1;
		while (*tokens && (*tokens)->type != TOKEN_PIPE)
			*tokens = (*tokens)->next;
		return ;
	}
	close(in_fd);
	free(cmd->infile);
	cmd->infile = ft_strdup((*tokens)->value);
	*tokens = (*tokens)->next;
}

char	*remove_surrounding_quotes_if_any(const char *str)
{
	size_t	len;
	char	*sub;

	len = ft_strlen(str);
	if (len >= 2)
	{
		if ((str[0] == '\'' && str[len - 1] == '\'')
			|| (str[0] == '"' && str[len - 1] == '"'))
		{
			sub = ft_substr(str, 1, len - 2);
			return (sub);
		}
	}
	return (ft_strdup(str));
}

int	check_heredoc_token_error(t_token **tokens, t_command *cmd)
{
	if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
	{
		ft_putstr_fd("Syntax error: missing delimiter for heredoc\n", 2);
		cmd->redir_error_code = 1;
		return (1);
	}
	return (0);
}

char	*get_effective_delim(const char *clean, int is_quoted, t_shell *shell)
{
	if (is_quoted)
		return (ft_strdup(clean));
	return (expand_heredoc_line(clean, shell));
}

char	*get_heredoc_clean(t_token **tokens, t_command *cmd, int *is_quoted)
{
	char	*raw;
	char	*clean;

	*tokens = (*tokens)->next;
	if (check_heredoc_token_error(tokens, cmd))
		return (NULL);
	raw = (*tokens)->value;
	*is_quoted = ((*tokens)->has_single_quote
			|| (*tokens)->has_double_quote);
	clean = remove_surrounding_quotes_if_any(raw);
	return (clean);
}
