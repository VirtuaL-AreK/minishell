/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanmazir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:08:38 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/06 13:24:25 by aanmazir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_unclosed_quotes(const char *input, t_shell *shell)
{
	int	single_quote;
	int	double_quote;
	int	i;

	single_quote = 0;
	double_quote = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' && double_quote == 0)
			single_quote = !single_quote;
		else if (input[i] == '"' && single_quote == 0)
			double_quote = !double_quote;
		i++;
	}
	if (single_quote || double_quote)
	{
		printf("Syntax error: unclosed quote\n");
		shell->exit_status = 2;
		return (1);
	}
	return (0);
}

int	check_initial_token(t_token *token, t_shell *shell)
{
	if (!token)
	{
		shell->exit_status = 2;
		return (1);
	}
	if (token->type == TOKEN_PIPE)
	{
		ft_putstr_fd("Syntax error near unexpected token '|'\n", 2);
		shell->exit_status = 2;
		return (1);
	}
	return (0);
}

void	synth_error(void)
{
	char	*msg;

	msg = "Syntax error: missing delimiter for heredoc\n";
	ft_putstr_fd(msg, 2);
}

int	check_token_error(t_token *token, t_shell *shell)
{
	if (token->type == TOKEN_PIPE)
	{
		if (!token->next || token->next->type == TOKEN_PIPE)
		{
			ft_putstr_fd("Syntax error near unexpected token '|'\n", 2);
			shell->exit_status = 2;
			return (1);
		}
	}
	else if (token->type == TOKEN_REDIR_IN || token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_APPEND || token->type == TOKEN_HEREDOC)
	{
		if (!token->next || token->next->type != TOKEN_WORD)
		{
			if (token->type == TOKEN_HEREDOC)
				synth_error();
			else
				ft_putstr_fd("Syntax error: missing file for redirection\n", 2);
			shell->exit_status = 2;
			return (1);
		}
	}
	return (0);
}

int	is_syntax_error(t_token *tokens, t_shell *shell)
{
	if (check_initial_token(tokens, shell))
		return (1);
	while (tokens)
	{
		if (check_token_error(tokens, shell))
			return (1);
		tokens = tokens->next;
	}
	return (0);
}
