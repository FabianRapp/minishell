CC=cc
CFLAGS=-Wall -Wextra -Werror

NAME=minishell

GENERAL_SOURCES=
GENERAL_OBJECTS=

SOURCES=repl/main.c
OBJECTS= $(SOURCES:.c=.o)
#$(GENERAL_OBJECTS)
LEXER_DIR = ./lexer
LIB_LEXER = lexer
LIB_LEXER_NAME = lexer.a
LEXER_PATH = $(LEXER_DIR)/$(LIB_LEXER_NAME)

LIBFT_DIR = ./libft
LIBFT = libft
LIBFT_NAME = libft.a
LIBFT_PATH = $(LIBFT_DIR)/$(LIBFT_NAME)

LIBS =  $(LIBFT) $(LIB_LEXER)
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
	@echo "\033[33mroot objects cleaned\033[0m"
#@$(MAKE) -C clean $(LIBFT_DIR)

fclean: clean
	@rm -f $(LIBFT_NAME) $(LIB_LEXER_NAME) a.out
	@$(MAKE) -C lexer fclean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "\033[33mroot fcleaned\033[0m"

re: fclean all

clean2:
	@rm -f $(OBJECTS)
	@echo "\033[33mObject files removed.\033[0m"

norm:
	norminette $(SOURCES)
