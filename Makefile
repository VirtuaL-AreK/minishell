CC = gcc
CFLAGS = -Wall -Wextra -Werror 
NAME = minishell
SRC = main.c utils.c syntax.c tokenization.c commands.c ./builtins/ft_echo.c ./builtins/ft_cd.c \
		execution.c ./builtins/ft_pwd.c
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