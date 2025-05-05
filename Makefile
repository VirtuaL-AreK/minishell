CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = minishell
SRC = main.c ./parsing/syntax.c ./parsing/split_tokens.c ./parsing/split_tokens_0.c ./parsing/split_tokens_01.c ./parsing/tokenization.c ./environment/env_utils.c \
		./builtins/ft_echo.c ./builtins/ft_cd.c ./execution/execution.c ./builtins/ft_pwd.c ./builtins/ft_export.c ./builtins/ft_unset.c ./builtins/ft_env.c \
		./builtins/ft_exit.c ./execution/signals.c ./execution/commands.c ./builtins/export_h/env_h_00.c ./builtins/export_h/env_h_01.c ./environment/shlvl.c \
		./builtins/export_h/env_h_02.c ./builtins/export_h/env_cd.c ./parsing/expansion.c ./execution/cmd/prepare_command.c ./execution/cmd/here_doc.c \
		./execution/exec/exec.c ./execution/exec/exec_built.c ./execution/exec/exec_built_2.c ./execution/exec/exec_cmd.c ./execution/exec/exec_redir.c \
		./execution/exec/exec_pipe.c ./parsing/exp/exp_env.c ./parsing/exp/pre_expansion.c ./builtins/export_h/env_cd_syn.c ./execution/cmd/here_doc_1.c \
		./execution/cmd/here_doc_2.c ./execution/cmd/here_doc_3.c ./execution/cmd/here_doc_4.c ./execution/cmd/here_doc_line.c ./parsing/exp/expand_state.c \
		./parsing/exp/exp_backslash.c ./parsing/exp/expand_string.c ./parsing/exp/expand_list_funcs.c ./parsing/exp/exp_env_1.c
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