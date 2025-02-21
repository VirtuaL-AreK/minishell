#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
# include "libft/libft.h"

#define RESET   "\x1B[0m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN    "\x1B[36m"
#define WHITE   "\x1B[37m"

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

// Linked list

typedef struct s_token
{
    char            *value;
    t_token_type    type;
    struct s_token  *next;
} t_token;

// utils

char	**ft_split(char const *s, char c);

int syntax_verification(char **args);
void tokenization(char **args);

#endif