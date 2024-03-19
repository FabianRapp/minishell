CC=cc





NAME=minishell

GENERAL_SOURCES=
GENERAL_OBJECTS=

# SOURCES = repl/main.c repl/utils/path.c repl/ft_buildin1.c repl/redir.c \
# 			repl/utils/input_exit.c repl/utils/data_utils.c repl/run_ast.c \
# 			utils/fd1.c \
# 			repl/expansion/expansion.c repl/expansion/utils1.c repl/expansion/word_splitting.c \
# 			repl/expansion/utils2.c \
# 			repl/expansion/wildcards/repl_wildcards_main.c repl/expansion/wildcards/repl_wildcards_utils1.c \
# 			repl/expansion/wildcards/repl_fill_wildcard_data.c \
# 			utils/utils3.c \
# 			repl/signals/child_parent_coms.c \
# 			utils/debugging.c \
# 			mk_adds/builtins/pwd.c mk_adds/builtins/env.c mk_adds/builtins/export.c mk_adds/builtins/unset.c \
# 			mk_adds/builtins/exit.c mk_adds/builtins/echo.c mk_adds/builtins/cd.c \
# 			mk_adds/env_stuff/env_functions.c

SOURCES = repl/main.c \
		  repl/utils/path.c \
		  repl/ft_buildin1.c \
		  repl/utils/input_exit.c \
		  repl/utils/data_utils.c \
		  repl/run_ast.c \
		  repl/env.c \
		  utils/fd1.c \
		  repl/repl_sub_shell.c \
		  repl/expansion/expansion.c \
		  repl/expansion/utils1.c \
		  repl/expansion/word_splitting.c \
		  repl/expansion/utils2.c \
		  repl/expansion/utils3.c \
		  repl/expansion/wildcards/repl_wildcards_main.c \
		  repl/expansion/wildcards/repl_wildcards_utils1.c \
		  repl/expansion/wildcards/repl_fill_wildcard_data.c \
		  utils/utils3.c \
		  repl/utils/repl_get_pid.c \
		  cleanup/cleanup1.c \
		  repl/signals/child_parent_coms.c \
		  utils/debugging.c \
		  utils/utils1.c \
		  utils/groups1.c \
		  builtins/pwd.c \
		  builtins/env.c \
		  builtins/export.c \
		  builtins/unset.c \
		  builtins/exit.c \
		  builtins/echo.c \
		  builtins/cd.c \
		  environment/initialize_env.c \
		  environment/modify_env.c \
		  environment/get_env_parts.c \
		  repl/repl_redir/repl_redir_main.c \
		  repl/repl_redir/repl_redir_utils.c \
		  repl/repl_redir/repl_redir_error_handler.c\
		  repl/pipes.c

OBJECTS= $(SOURCES:.c=.o)

#$(GENERAL_OBJECTS)
LEXER_DIR = ./lexer
export LIB_LEXER = lexer
LIB_LEXER_NAME = lexer.a
LEXER_PATH = $(LEXER_DIR)/$(LIB_LEXER_NAME)

NAME_TESTER=

LIBFT_DIR = ./libft
export LIBFT = libft
LIBFT_NAME = libft.a
LIBFT_PATH = $(LIBFT_DIR)/$(LIBFT_NAME)

PARSER_DIR = ./parser
export LIB_PARSER = parser
LIB_PARSER_NAME = parser.a
PARSER_PATH = $(PARSER_DIR)/$(LIB_PARSER_NAME)

LIBS = $(LIBFT) $(LIB_LEXER) $(LIB_PARSER)
LIBS_NAME = $(LIBFT_NAME) $(LIB_LEXER_NAME) $(LIB_PARSER_NAME)



.PHONY: all clean fclean re clean2 libs $(LIBFT) $(LIB_LEXER) $(LIB_PARSER)

all: normal

normal: CFLAGS += $(FLAGS_NO_LEAK_CHECK)
normal: LDFLAGS += $(FLAGS_NO_LEAK_CHECK)
normal: $(OBJECTS) libs
	$(CC) $(LIBS_NAME) $(OBJECTS)  -lreadline -o $(NAME) $(CFLAGS) $(LDFLAGS)
	cp $(NAME) ../../../bash_testing


$(NAME): $(OBJECTS)
	$(CC) $(LIBS_NAME) $(OBJECTS)  -lreadline -o $(NAME) $(CFLAGS) $(LDFLAGS)
	cp $(NAME) ../../../bash_testing


leaks: CFLAGS += -DLEAK_CHECK=1
leaks: libs_leaks
leaks: $(OBJECTS)
leaks: $(NAME)

#leaks: fclean

libs: $(LIBS)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)
	@cp $(LIBFT_PATH) $(LIBFT_NAME)

$(LIB_LEXER):
	@$(MAKE) -C $(LEXER_DIR)
	@cp $(LEXER_PATH) $(LIB_LEXER_NAME)

$(LIB_PARSER):
	@$(MAKE) -C $(PARSER_DIR)
	@cp $(PARSER_PATH) $(LIB_PARSER_NAME)

libs_leaks:
	@$(MAKE) -C $(LIBFT_DIR) leaks
	@cp $(LIBFT_PATH) $(LIBFT_NAME)
	@$(MAKE) -C $(LEXER_DIR) $(LIB_LEXER) leaks
	@cp $(LEXER_PATH) $(LIB_LEXER_NAME)
	@$(MAKE) -C $(PARSER_DIR) $(LIB_PARSER) leaks
	@cp $(PARSER_PATH) $(LIB_PARSER_NAME)

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $^

clean:
	@rm -f $(OBJECTS)
	@$(MAKE) -C $(LEXER_DIR) clean
	@$(MAKE) -C $(LIBFT_DIR) clean
	@$(MAKE) -C $(PARSER_DIR) clean
	@echo "\033[33mroot objects cleaned\033[0m"

#TODO add cleaning of testers
fclean: clean
	@rm -f $(LIB_LEXER_NAME) $(LIBFT_NAME) $(NAME_TESTER) $(LIB_PARSER_NAME) $(NAME) a.out
	@rm -f $(LIBFT_PATH)
	@rm -f $(LEXER_PATH) $(LEXER_DIR)/$(LIBFT_NAME) $(LEXER_DIR)/a.out
	@rm -f $(PARSER_PATH) $(PARSER_DIR)/$(LIBFT_NAME) $(PARSER_DIR)/$(LIB_PARSER_NAME) a.out
	@echo "\033[33mroot fcleaned\033[0m"

re: fclean all

norm:
	norminette $(SOURCES)
