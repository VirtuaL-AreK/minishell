CC = cc
FLAGS = -Wall -Wextra -Werror
NAME = libft.a
AR = ar rcs
RM = rm -rf
SRCS = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c \
       ft_isprint.c ft_strlen.c ft_memset.c ft_bzero.c \
       ft_memcpy.c ft_memmove.c ft_strlcpy.c ft_strlcat.c \
       ft_calloc.c ft_strdup.c ft_toupper.c ft_tolower.c \
       ft_strchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c \
       ft_strnstr.c ft_atoi.c ft_strrchr.c ft_substr.c \
       ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c \
       ft_strmapi.c ft_striteri.c ft_putchar_fd.c \
       ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c get_next_line.c\
	   
BONUS = ft_lstnew_bonus.c ft_lstadd_front_bonus.c \
		ft_lstsize_bonus.c ft_lstlast_bonus.c \
		ft_lstadd_back_bonus.c ft_lstdelone_bonus.c \
		ft_lstclear_bonus.c ft_lstiter_bonus.c \
		ft_lstmap_bonus.c 
OBJ = $(SRCS:.c=.o)
OBJB = $(BONUS:.c=.o)

.PHONY: all clean bonus fclean re

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(NAME) $(OBJ) 

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

bonus : $(NAME) $(OBJB)
	$(AR) $(NAME) $(OBJB)

clean :
	$(RM) $(OBJ) $(OBJB)

fclean: clean
	$(RM) $(NAME) $(bonus)

re : fclean all