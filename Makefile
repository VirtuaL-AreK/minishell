CC = gcc
CFLAGS = -Wall -Wextra -Werror 
NAME = minishell
SRC = main.c utils.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) 
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lreadline

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re