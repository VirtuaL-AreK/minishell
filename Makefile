CC = cc
CFLAGS = -Wall -Wextra -Werror 
NAME = minishell
SRC = main.c utils.c syntax.c split_tokens.c split_tokens_0.c split_tokens_01.c tokenization.c env_utils.c commands.c expansion.c \
		./builtins/ft_echo.c ./builtins/ft_cd.c execution.c ./builtins/ft_pwd.c ./builtins/ft_export.c ./builtins/ft_unset.c ./builtins/ft_env.c \
		./builtins/ft_exit.c ./execution/signals.c ./execution/exit_status.c ./builtins/export_h/export_h_00.c ./builtins/export_h/export_h_01.c
OBJ = $(SRC:.c=.o)
LIBFT = ./libft/libft.a

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME) -lreadline

$(LIBFT):
	$(MAKE) -C ./libft

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) clean -C ./libft
	rm -rf $(OBJ)

fclean: clean
	$(MAKE) fclean -C ./libft
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re