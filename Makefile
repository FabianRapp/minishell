CC=cc
CFLAGS=-Wall -Wextra -Werror -g  -fsanitize=address
#-fsanitize=undefined 
# -g  
LDFLAGS = -fsanitize=address
#
NAME=minishell

GENERAL_SOURCES=
GENERAL_OBJECTS=

SOURCES=repl/main.c  repl/utils/path.c repl/ft_buildin1.c repl/redir.c \
 repl/utils/input_exit.c repl/utils/data_utils.c repl/run_ast.c repl/env.c \
 utils/fd1.c \
 repl/expansion/expansion.c repl/expansion/utils1.c repl/expansion/word_splitting.c \
 repl/expansion/utils2.c repl/expansion/utils3.c\
\
 repl/signals/child_parent_coms.c

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

all: $(NAME)

$(NAME): libs $(OBJECTS)
	$(CC) $(LIBS_NAME) $(OBJECTS)  -lreadline -o $(NAME) $(CFLAGS) $(LDFLAGS)
	cp $(NAME) ../../../bash_testing


libs: $(LIBS)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)
	@cp $(LIBFT_PATH) $(LIBFT_NAME)

$(LIB_LEXER):
	@$(MAKE) -C $(LEXER_DIR) $(LIB_LEXER)
	@cp $(LEXER_PATH) $(LIB_LEXER_NAME)

$(LIB_PARSER):
	@$(MAKE) -C $(PARSER_DIR) $(LIB_PARSER)
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
