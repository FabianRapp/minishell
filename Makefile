NAME	=	minishell

CFLAGS	=	-Wall -Wextra -Werror
CC		=	clang
LIBFT 	=	libft/libft.a

GREEN	=	\033[0;32m
YELLOW	=	\033[33m
CYAN	=	\033[0;36m
CLEAR	=	\033[0m

SRC_BUILTINS	=	builtins/pwd.c builtins/env.c builtins/export.c \
					builtins/unset.c builtins/exit.c builtins/echo.c \
					builtins/cd.c builtins/cd_utils.c builtins/builtin_control.c

SRC_ENVIRONMENT =	environment/initialize_env.c environment/modify_env.c \
					environment/get_env_parts.c

SRC_LEXER		=	lexer/lexer_main.c lexer/lexer_utils.c lexer/dollar_sign.c \
					lexer/fd_utils.c lexer/ident_redir.c lexer/ident_type.c \
					lexer/lexer_ident_literal_wildcards.c lexer/subshell.c

SRC_PARSER		=	parser/parser.c parser/parser_type_commands.c \
					parser/parse_utils.c parser/parser_ast/parser_ast.c \
					parser/parser_ast/parser_ast_redir.c \
					parser/parser_ast/parser_ast_here_doc.c \
					parser/parser_ast/parser_ast_utils1.c \
					parser/parser_ast/parser_ast_utils2.c \
					parser/list_operations/add_new.c \
					parser/list_operations/move_nodes.c \
					parser/list_operations/remove_nodes.c \
					parser/parser_args_redirs.c \
					parser/list_operations/utils.c

SRC_EXPANSION	=	expansion/expansion.c expansion/word_splitting.c \
					expansion/utils1.c expansion/utils2.c \
					expansion/wildcards/repl_fill_wildcard_data.c \
					expansion/wildcards/repl_wildcards_main.c \
					expansion/wildcards/repl_wildcards_utils1.c \
					expansion/utils3.c

SRC_EXECUTION	=	execution/redirections/redir_error_handler.c \
					execution/run_command.c \
					execution/redirections/redir_main.c \
					execution/redirections/redir_utils.c \
					execution/pipes.c execution/run_ast.c \
					execution/exec_subshell.c execution/utils/pipe_utils.c \
					execution/utils/data_utils.c execution/utils/input_exit.c \
					execution/utils/path.c execution/utils/path_utils.c \
					execution/utils/get_pid.c

SRC_UTILS		=	utils/signals.c utils/debugging.c utils/fd1.c utils/groups1.c \
					utils/groups2.c utils/get_state.c utils/cleanup2.c \
					utils/utils.c utils/exit_state.c utils/cleanup.c \
					utils/error_handlers.c utils/alloc_utils.c \
					utils/type_to_error.c\
					utils/signals2.c

SRCS	=	$(SRC_BUILTINS) $(SRC_ENVIRONMENT) $(SRC_LEXER) $(SRC_PARSER) \
			$(SRC_EXPANSION) $(SRC_EXECUTION) $(SRC_UTILS) main.c

OBJS	=	$(SRCS:%.c=%.o)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@cd libft && make
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)
	@echo "$(GREEN)minishell compiled!$(CLEAR)"

all: $(NAME)

flags: CFLAGS += -fsanitize=undefined -fsanitize=address -g
flags: LDFLAGS += -fsanitize=undefined -fsanitize=address -g
flags: $(OBJS)
	@cd libft && make flags
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)
	@echo "$(GREEN)minishell compiled!$(CLEAR)"

leaks: CFLAGS += -DLEAK_CHECK=1
leaks: $(OBJS)
	@cd libft && make leaks
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)
	@echo "$(GREEN)minishell compiled!$(CLEAR)"

clean:
	@cd libft && make clean
	@rm -f $(OBJS)
	@echo "$(CYAN)object files cleaned$(CLEAR)"

fclean:
	@cd libft && make fclean
	@rm -f $(OBJS)
	@rm -f $(NAME)
	@echo "$(CYAN)minishell fclean$(CLEAR)"

re: fclean all

.PHONY: all clean fclean re
