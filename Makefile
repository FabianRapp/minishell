FLAGS_NO_LEAK_CHECK = 
#  
CFLAGS=-Wall -Wextra -Werror

LDFLAGS =
# -fsanitize=undefined -fsanitize=address -g
# 


NAME	=	minishell

CFLAGS	=	-Wall -Wextra -Werror
CC		=	cc
LIBFT 	=	libft/libft.a

GREEN	=	\033[0;32m
YELLOW	=	\033[33m
CYAN	=	\033[0;36m
CLEAR	=	\033[0m

SRC_BUILTINS	=	builtins/pwd.c builtins/env.c builtins/export.c \
					builtins/unset.c builtins/exit.c builtins/echo.c \
					builtins/cd.c builtins/cd_utils.c

SRC_ENVIRONMENT =	environment/initialize_env.c environment/modify_env.c \
					environment/get_env_parts.c

SRC_LEXER		=	lexer/lexer_main.c lexer/lexer_utils.c lexer/dollar_sign.c \
					lexer/fd_utils.c lexer/ident_redir.c lexer/ident_type.c \
					lexer/lexer_ident_literal_wildcards.c lexer/subshell.c

SRC_PARSER		=	parser/parser.c parser/parser_type_commands.c \
					parser/parse_utils.c parser/parser_ast/parser_ast.c \
					parser/parser_ast/parser_ast_redir.c \
					parser/parser_ast/parser_ast_utils1.c \
					parser/parser_ast/parser_ast_utils2.c \
					parser/list_operations/add_new.c \
					parser/list_operations/move_nodes.c \
					parser/list_operations/remove_nodes.c \
					parser/list_operations/utils.c

SRC_EXPANSION	=	expansion/expansion.c expansion/word_splitting.c \
					expansion/utils1.c expansion/utils2.c expansion/utils3.c \
					expansion/wildcards/repl_fill_wildcard_data.c \
					expansion/wildcards/repl_wildcards_main.c \
					expansion/wildcards/repl_wildcards_utils1.c

SRC_EXECUTION	=	execution/redirections/redir_error_handler.c \
					execution/redirections/redir_main.c \
					execution/redirections/redir_utils.c \
					execution/pipes.c execution/run_ast.c \
					execution/exec_buildin.c execution/exec_subshell.c \
					execution/utils/data_utils.c execution/utils/path.c \
					execution/utils/input_exit.c execution/utils/get_pid.c \
					execution/utils/pipe_utils.c

SRC_UTILS		=	utils/signals.c utils/debugging.c utils/fd1.c utils/groups1.c \
					utils/utils1.c utils/utils2.c utils/cleanup.c

SRCS	:=	$(SRC_BUILTINS) $(SRC_ENVIRONMENT) $(SRC_LEXER) $(SRC_PARSER) \
			$(SRC_EXPANSION) $(SRC_EXECUTION) $(SRC_UTILS) main.c

OBJS	=	$(SRCS:%.c=%.o)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@cd libft && make
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)
	@echo "$(GREEN)minishell compiled!$(WHITE)"

all: $(NAME)

flags: CFLAGS += -fsanitize=undefined -fsanitize=address -g
flags: LDFLAGS += -fsanitize=undefined -fsanitize=address -g
flags: $(OBJS)
	@cd libft && make flags
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)
	@echo "$(GREEN)minishell compiled!$(WHITE)"

leaks: CFLAGS += -DLEAK_CHECK=1
leaks: $(OBJS)
	@cd libft && make leaks
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)
	@echo "$(GREEN)minishell compiled!$(WHITE)"

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







# OBJS	=	$(SRCS:%.c=$(OBJ_DIR)%.o)

# LIBFT	=	libft/libft.a

# $(NAME): $(OBJS)
# 	@cd libft && make
# 	@$(CC) $(CFLAGS) $(LIBFT) $(OBJS) -lreadline -o $(NAME) $(LDFLAGS)
# 	@echo "$(GREEN)minishell build$(WHITE)"

# $(OBJ_DIR)%.o: %.c
# 	@mkdir -p $(OBJ_DIR)
# 	@$(CC) $(CFLAGS) -c $< -o $@


# all:	$(NAME)

# clean:	
# 		rm -f $(OBJS)
# 		@cd libft && make clean
# 		@rm -rf $(OBJ_DIR)
# 		@echo "$(CYAN)object files cleaned!$(WHITE)"

# fclean:	
# 		@cd libft && make fclean
# 		@rm -rf $(OBJ_DIR)
# 		@rm -f $(NAME)
# 		@echo "$(CYAN)Executable and object files cleaned!$(WHITE)"

# re:		fclean all

# .PHONY: all clean fclean re