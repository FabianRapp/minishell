CC=cc
CFLAGS=-Wall -Wextra -Werror -g
#-fsanitize=address
# -g  -fsanitize=undefined
LDFLAGS = 
#-fsanitize=address
NAME=minishell

GENERAL_SOURCES=
GENERAL_OBJECTS=

SOURCES=repl/main.c
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

LIBS = $(LIBFT) $(LIB_LEXER) 
LIBS_NAME = $(LIBFT_NAME) $(LIB_LEXER_NAME) 

.PHONY: all clean fclean re clean2 libs $(LIBFT) $(LIB_LEXER)

all: $(NAME)

$(NAME): libs $(OBJECTS)
	$(CC) $(LIBFT_NAME) $(LIB_LEXER_NAME) $(OBJECTS)  -lreadline -o $(NAME)


libs: $(LIBS)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)
	@cp $(LIBFT_PATH) $(LIBFT_NAME)

$(LIB_LEXER):
	@$(MAKE) -C $(LEXER_DIR) $(LIB_LEXER)
	@cp $(LEXER_PATH) $(LIB_LEXER_NAME)

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
	@rm -f $(LIB_LEXER_NAME) $(LIBFT_NAME) $(NAME_TESTER) $(NAME) a.out
	@rm -f $(LIBFT_PATH)
	@rm -f $(LEXER_PATH) $(LEXER_DIR)/$(LIBFT_NAME) $(LEXER_DIR)/a.out
	@rm -f $(PARSER_PATH) $(PARSER_DIR)/$(LIBFT_NAME) $(PARSER_DIR)/$(LIB_LEXER_NAME) a.out
	@echo "\033[33mroot fcleaned\033[0m"

re: fclean all

norm:
	norminette $(SOURCES)
