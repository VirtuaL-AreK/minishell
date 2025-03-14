#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
# include "libft/libft.h"

#include <sys/stat.h>
#include <sys/types.h>

#define RESET   "\x1B[0m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN    "\x1B[36m"
#define WHITE   "\x1B[37m"

typedef struct s_shell {
    char **env;
    int exit_status;
} t_shell;

typedef struct s_strlist {
    char   *str;
    int    has_single_quote;
    int    has_double_quote;
    struct s_strlist *next;
} t_strlist;

extern t_shell g_shell;

typedef enum e_token_type
{
    TOKEN_WORD = 0,      // Word (command, argument, filename)
    TOKEN_PIPE = 1,      // "|"
    TOKEN_REDIR_IN = 2,  // "<"
    TOKEN_REDIR_OUT = 3, // ">"
    TOKEN_APPEND = 4,    // ">>"
    TOKEN_HEREDOC = 5,   // "<<"
    TOKEN_SPACE = 6,     // Whitespace
    TOKEN_EOF = 7        // End of input
} t_token_type;

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
} t_exit_status;

// extern t_shell g_shell;

typedef struct s_token {
    char *value;
    t_token_type type;
    int has_single_quote;
    int has_double_quote;
    struct s_token *next;
} t_token;


// Commands
typedef struct s_command {
	int nb_arg;
    char **args;          // Arguments of the command within pipes
    char *infile;         // Input file if '<'
    char *outfile;        // Output file si '>' ou '>>'
    int append;           // 1 if '>>', 0 else
    int redir_error_code;
    // int is_single_quote;
	struct s_command *next; // Next command if '|' exist
} t_command;

// utils
char	**ft_split(char const *s, char c);
void ft_free_args(char **args);

// 1) Gestion de l'environnement local
char **clone_envp(char **envp);
void  free_envp(char **envp);

// syntax
int check_unclosed_quotes(const char *input);

// split_tokens
// void skip_spaces(const char *input, int *i);
int is_special(char c);
int count_tokens(const char *input);
char **ft_split_command(const char *input);

// tokenizatiion
t_token *bash_tokenize_to_tokens(const char *input);
t_token_type get_token_type(char *str);
void free_tokens(t_token *head);
void print_tokens(t_token *head);
int syntax_verification(char **args);
t_token *tokenization(char **args);

int check_quotes(char *str);

// commands

void print_command(t_command *commands);
t_command *command_parser(t_token *tokens);
void free_commands(t_command *cmd);

// expansion
char *add_or_replace_var(t_shell *shell, const char *name, const char *value);
void expand_tokens(t_token *tokens, t_shell *shell);

// execution

char *find_exec(char *cmd);
void execute_command(char **args, char **env);
int  execute_builtin(t_command *cmd, t_shell *shell);
void parse_command(char *input, t_shell *shell);
void execute_pipeline(t_command *cmd, t_shell *shell);
void execute_command_line(t_command *cmd, char **env);

// Builtins

int ft_env(t_command *cmd, t_shell *shell);
int ft_export(t_command *cmd, t_shell *shell);
int ft_unset(t_command *cmd, t_shell *shell);
int ft_cd(t_command *cmd, t_shell *shell);
int ft_echo(t_command *cmd, t_shell *shell);
int ft_exit(t_command *cmd, t_shell *shell);
int ft_pwd(t_command *cmd, t_shell *shell);


int  check_unclosed_quotes(const char *input);
void prompt_loop(t_shell *shell);

// Signals
void sig_handler(int sig);

void exit_here(t_exit_status order);

#endif