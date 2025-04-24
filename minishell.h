/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-kher <iel-kher@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 10:21:52 by aanmazir          #+#    #+#             */
/*   Updated: 2025/04/24 12:29:35 by iel-kher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <signal.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include <sys/ioctl.h>
# include "libft/libft.h"

# include <sys/stat.h>
# include <sys/types.h>

# define RESET   "\x1B[0m"
# define RED     "\x1B[31m"
# define GREEN   "\x1B[32m"
# define YELLOW  "\x1B[33m"
# define BLUE    "\x1B[34m"
# define MAGENTA "\x1B[35m"
# define CYAN    "\x1B[36m"
# define WHITE   "\x1B[37m"

typedef struct s_shell
{
	char	**env;
	char	*path;
	int		exit_status;
	int		heredoc_interrupted;
}	t_shell;

typedef struct s_heredoc_ctx
{
	int					fd;
	int					is_quoted;
	const char			*delimiter;
	t_shell				*shell;
}	t_heredoc_ctx;

typedef struct s_strlist
{
	char				*str;
	int					has_single_quote;
	int					has_double_quote;
	int					should_expand;
	struct s_strlist	*next;
}	t_strlist;

typedef struct s_token_flags
{
	int	has_sq;
	int	has_dq;
	int	should_expand;
}	t_token_flags;

extern volatile sig_atomic_t	g_last_signal;

typedef struct s_export_parts
{
	char				*name;
	char				*to_append;
}	t_export_parts;

typedef struct s_expand_state
{
	char				*buffer;
	int					idx;
	int					capacity;
	int					in_sq;
	int					in_dq;
}	t_expand_state;

typedef enum e_token_type
{
	TOKEN_WORD = 0,
	TOKEN_PIPE = 1,
	TOKEN_REDIR_IN = 2,
	TOKEN_REDIR_OUT = 3,
	TOKEN_APPEND = 4,
	TOKEN_HEREDOC = 5,
	TOKEN_SPACE = 6,
	TOKEN_EOF = 7
}	t_token_type;

// GENERAL EXIT STATUS
typedef enum s_exit_status
{
	SUCCESS = 0,
	GENERAL_ERROR = 1,
	MISUSE_BUILTINS = 2,
	CANNOT_EXECUTE = 126,
	CMD_NOT_FOUND = 127,
	SIGNAL_TERM = 128,
	SIGINT_RECEIVED = 130,
	SIGQUIT_RECEIVED = 131
}	t_exit_status;

// extern t_shell g_shell;
typedef struct s_token
{
	char				*value;
	t_token_type		type;
	int					has_single_quote;
	int					has_double_quote;
	int					should_expand;
	struct s_token		*next;
}	t_token;

// Commands
typedef struct s_command
{
	int					nb_arg;
	char				**args;
	char				*infile;
	char				*outfile;
	int					append;
	int					redir_error_code;
	struct s_command	*next;
}	t_command;

typedef struct s_pipe_ctrl
{
	t_shell				*shell;
	pid_t				*pids;
	int					*idx;
	int					prev_fd;
	int					pipe_fd[2];
	int					has_pipe;
}	t_pipe_ctrl;

typedef struct s_prs_ctx
{
	const char		*line;
	int				*i;
	char			**buf;
	int				*len;
	int				*cap;
}	t_prs_ctx;

typedef struct s_exp_ctx
{
	const char			*s;
	int					idx;
	int					in_sq;
	int					in_dq;
	t_expand_state		*st;
	t_shell				*shell;
}	t_exp_ctx;

void			print_error(const char *what);

void			print_command(t_command *commands);
int				count_command_arguments(t_token *tokens);
t_command		*new_command(t_token *tokens);

// char			*handle_heredoc(const char *delimiter);
char			*handle_heredoc(const char *delimiter,
					int is_quoted, t_shell *shell);
void			handle_word(t_command *cmd,
					t_token **tokens, int *arg_count);

//here doc
void			handle_redir_in(t_command *cmd, t_token **tokens);
void			handle_heredoc_token(t_command *cmd, t_token **tokens,
					t_shell *shell);
void			handle_redir_out_or_append(t_command *cmd,
					t_token **tokens);
void			fill_command(t_command *cmd, t_token **tokens, t_shell *shell);
void			fix_empty_first_arg(t_command *cmd);
char			*expand_heredoc_line(const char *line, t_shell *shell);
int				open_heredoc_tmp(char **out_template, int *out_fd);
char			*heredoc_on_interrupt(t_shell *shell, char *template, int fd);
void			write_heredoc_line(t_heredoc_ctx *ctx, const char *s);
int				handle_heredoc_line(t_heredoc_ctx *ctx, char *line);
void			sigint_handler_heredoc(int sig);
void			init_heredoc_signals(struct sigaction *old_sa);
void			restore_heredoc_signals(struct sigaction *old_sa);
int				create_heredoc_file(char *template);
int				read_heredoc_and_write(t_heredoc_ctx *ctx);
char			*handle_heredoc(const char *delimiter, int is_quoted,
					t_shell *shell);
int				handle_heredoc_failure(char *tmp, t_command **pcmd,
					t_token **ptokens, t_shell *shell);
void			handle_heredoc_token(t_command *cmd,
					t_token **tokens, t_shell *shell);
void			handle_redir_out_or_append(t_command *cmd, t_token **tokens);
void			fill_command(t_command *cmd, t_token **tokens, t_shell *shell);
void			fix_empty_first_arg(t_command *cmd);
void			handle_redir_in(t_command *cmd, t_token **tokens);
char			*remove_surrounding_quotes_if_any(const char *str);
int				check_heredoc_token_error(t_token **tokens, t_command *cmd);
char			*get_effective_delim(const char *clean, int is_quoted,
					t_shell *shell);
char			*get_heredoc_clean(t_token **tokens, t_command *cmd,
					int *is_quoted);
char			*expand_heredoc_line(const char *line, t_shell *shell);
void			append_char_result(char c, char **result);
int				process_dollar_in_heredoc(const char *line, int i,
					char **result, t_shell *shell);

// utils
char			**ft_split(char const *s, char c);
void			ft_free_args(char **args);
int				is_special_char(int c);
void			skip_spaces(const char *line, int *i);
void			add_strlist(t_strlist **head,
					const char *value, t_token_flags flags);

int				append_str(char **buf, int *len, int *cap, const char *s);
int				append_char(char **buf, int *len, int *cap, char c);

int				process_single_quote(t_prs_ctx *ctx);
int				process_double_quote(t_prs_ctx *ctx);
int				process_unquoted_char(t_prs_ctx *ctx);
char			*parse_one_token_merge_quotes(const char *line,
					int *i, t_token_flags *flags);
void			process_special_char_token(const char *line,
					int *i, t_strlist **result);

// 1) Gestion de l'environnement local
char			**clone_envp(char **envp, t_shell *shell);
void			free_envp(char **envp);

// syntax
int				check_unclosed_quotes(const char *input, t_shell *shell);
int				check_initial_token(t_token *token, t_shell *shell);
int				check_token_error(t_token *token, t_shell *shell);
int				is_syntax_error(t_token *tokens, t_shell *shell);

// split_tokens
// void skip_spaces(const char *input, int *i);
int				is_special(char c);
int				count_tokens(const char *input);
char			**ft_split_command(const char *input);

// tokenizatiion
t_token			*bash_tokenize_to_tokens(const char *input);
t_token_type	get_token_type(char *str);
void			free_tokens(t_token *head);
void			print_tokens(t_token *head);
int				syntax_verification(char **args);
t_token			*tokenization(char **args);
int				check_quotes(char *str);

// commands
void			print_command(t_command *commands);
t_command		*command_parser(t_token *tokens, t_shell *shell);
void			free_commands(t_command *cmd);

// expansion
int				init_expand_state(t_expand_state *st, int init_cap);
int				expand_buffer_if_needed(t_expand_state *st, int needed);
int				expand_add_char(t_expand_state *st, char c);
int				expand_add_string(t_expand_state *st, const char *s);
int				process_dollar_branch(const char *str, int i,
					t_expand_state *state, t_shell *shell);
int				process_backslash_branch(const char *str, int i,
					t_expand_state *state);
char			*add_or_replace_var(t_shell *shell,
					const char *name, const char *value);
void			expand_tokens(t_token *tokens, t_shell *shell);
char			*expand_string(const char *str, t_shell *shell);
char			*get_local_env_value(const char *var, t_shell *shell);
int				handle_alphanum_variable(const char *s, int *i,
					t_expand_state *st, t_shell *shell);
int				handle_variable(const char *s, int *i, t_expand_state *st,
					t_shell *shell);
int				handle_dollar_quoted(const char *s, int *i,
					t_expand_state *st, t_shell *shell);
char			process_escape_char(const char *s, int *i, char quote);
void			process_backslash(const char *s, int *i, int *j,
					char *result, char quote);
char			*process_ansi_c(const char *s);
char			*process_dollar_dquote(const char *s);
int				append_string(t_expand_state *st, const char *s);
int				init_expand_state(t_expand_state *st, int init_cap);
int				expand_buffer_if_needed(t_expand_state *st, int needed);
int				expand_add_char(t_expand_state *st, char c);
int				expand_add_string(t_expand_state *st, const char *s);
int				process_dollar_branch(const char *str, int i,
					t_expand_state *state, t_shell *shell);
int				process_backslash_branch(const char *str, int i,
					t_expand_state *state);
int				handle_dollar_expand(const char *str,
					int *i, t_expand_state *st, t_shell *shell);
int				handle_quote_toggle(const char *str, int i, t_expand_state *st);
int				handle_backslash_expand(const char *str, int i,
					t_expand_state *st);
int				handle_regular_char(const char *str, int i, t_expand_state *st);
int				process_char_in_expand(const char *str,
					int i, t_expand_state *st, t_shell *shell);
char			*expand_string(const char *str, t_shell *shell);
char			*expand_tilde(const char *value, t_shell *shell);
char			*remove_quotes(const char *s);
char			*free_and_null(char *buf);
int				toggle_quote(t_exp_ctx *ctx);
int				handle_dollar(t_exp_ctx *ctx);
int				handle_escape(t_exp_ctx *ctx);
int				handle_tilde(t_exp_ctx *ctx);

// execution
char			*find_exec(char *cmd, char **env);
void			execute_command(char **args, char **env);
int				execute_builtin(t_command *cmd, t_shell *shell);
void			parse_command(char *input, t_shell *shell);
void			execute_pipeline(t_command *cmd, t_shell *shell);
void			execute_command_line(t_command *cmd, char **env);
void			ft_free_strarray(char **arr);
void			execute_pipeline_child(t_command *c, t_pipe_ctrl *ctrl);
void			ft_free_strarray(char **arr);
char			*get_env_path(char **env);
char			*search_in_paths(char *env_path, char *cmd);
char			*search_in_cwd(char *cmd);
char			*find_exec(char *cmd, char **env);
int				is_critical_builtin(const char *cmd);
int				execute_builtin_cd(t_command *cmd, t_shell *shell);
int				execute_builtin_echo(t_command *cmd, t_shell *shell);
int				execute_builtin_exit(t_command *cmd, t_shell *shell);
int				execute_builtin_export(t_command *cmd, t_shell *shell);
int				execute_builtin_pwd(t_command *cmd, t_shell *shell);
int				execute_builtin_unset(t_command *cmd, t_shell *shell);
int				execute_builtin_xargs(t_command *cmd, t_shell *shell);
int				execute_builtin(t_command *cmd, t_shell *shell);
void			execute_command_child(char **args, char **env);
void			execute_command(char **args, char **env);
int				count_commands(t_command *cmd);
void			execute_command_exec(t_command *c, t_shell *shell);
void			setup_input_redirection(t_command *c, int prev_fd);
void			setup_output_redirection(t_command *c,
					int has_pipe, int pipe_fd[2]);
void			setup_redirection(t_command *c, int prev_fd,
					int pipe_fd[2], int has_pipe);

void			check_directory_and_permissions(const char *path);
void			wait_for_pipeline(pid_t *pids, int nb_cmds, t_shell *shell);

int				create_pipe_for_command(t_command *c, int pipe_fd[2]);
int				fork_pipeline_command(t_command *c, t_pipe_ctrl *ctrl);
int				handle_pipeline_command(t_command *c, t_pipe_ctrl *ctrl);
void			process_pipeline(t_command *cmd, t_shell *shell);

// Builtins
int				ft_env(t_command *cmd, t_shell *shell);
int				ft_export(t_command *cmd, t_shell *shell);
int				ft_unset(t_command *cmd, t_shell *shell);
int				ft_cd(t_command *cmd, t_shell *shell);
int				ft_echo(t_command *cmd, t_shell *shell);
int				ft_exit(t_command *cmd, t_shell *shell);
int				ft_pwd(t_command *cmd, t_shell *shell);
void			prompt_loop(t_shell *shell);
char			*ft_strjoin3(const char *s1, const char *s2, const char *s3);
int				find_var_index(t_shell *shell, const char *name, int len);
int				count_env(t_shell *shell);
char			*append_var(t_shell *shell, char *new_var, int count);
char			*add_or_replace_var(t_shell *shell, const char *name,
					const char *value);
int				is_valid_varname(char *var);
int				export_var_plus_get_parts(const char *arg, char *plus_eq,
					t_export_parts *parts, t_shell *shell);
char			*export_var_plus_get_old_val(t_shell *shell, const char *name);
void			export_var_plus_update(t_shell *shell, const char *name,
					const char *to_append, char *old_val);
void			export_var_plus(t_shell *shell, const char *arg, char *plus_eq);
int				extract_name_value(const char *arg, char *eq,
					char **name, char **value);
void			export_var_eq(t_shell *shell, const char *arg, char *eq);
void			export_var_no_eq(t_shell *shell, const char *arg);
void			export_var(t_shell *shell, const char *arg);
int				count_env_vars(t_shell *shell);
char			*get_home_from_env(t_shell *shell);
char			*get_oldpwd_from_env(t_shell *shell);
char			*resolve_cd_path_tilde(t_command *cmd, t_shell *shell);
char			*resolve_cd_path_dash(t_command *cmd, t_shell *shell);
char			*resolve_cd_path_home_slash(t_command *cmd, t_shell *shell);
char			*resolve_cd_double_dash(t_command *cmd, t_shell *shell);
char			*resolve_cd_path(t_command *cmd, t_shell *shell);

// Signals
void			sig_handler(int sig);
void			exit_here(t_exit_status order);
void			sigint_handler_prompt(int sig);

char			*expand_tilde(const char *value, t_shell *shell);

//Shell Level Environment
int				find_shlvl_index(char **env);
char			*format_shlvl(int lvl);
char			**handle_shlvl(char **env, t_shell *shell);

#endif
