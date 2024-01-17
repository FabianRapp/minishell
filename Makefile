CC=cc
CFLAGS=-Wall -Wextra -Werror

GENERAL_INCLUDES=-I../main_headers -I../utils -I../libft
GENERAL_SOURCES=

LEXER_DIR = ./lexer
LEXER = lexer.a
LEXER_PATH = $(LIBFT_DIR)/$(LIBFT)

LIBFT_DIR = ./libft
LIBFT = libft.a
LIBFT_PATH = $(LIBFT_DIR)/$(LIBFT)


.PHONY: all clean fclean re clean2

all:$(LIBFT) $(LIB_LEXER)

$(LIB_LEXER):
	@$(MAKE) -C $(LEXER_DIR)
	@cp $(LEXER_PATH) $(LEXER)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)
	@cp $(LIBFT_PATH) $(LIBFT)

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) $(INCLUDES_TESTER) -o $@ -c $^

clean:
	@rm -f $(OBJECTS) $(OBJECTS_TESTER)
	@echo "\033[33mObject files removed.\033[0m"

fclean: clean
	@rm -f $(LIB_LEXER) $(LIBFT) $(LIB_LEXER) a.out
	@echo "\033[33mExecutable removed.\033[0m"

re: fclean all

clean2:
	@rm -f $(OBJECTS)
	@echo "\033[33mObject files removed.\033[0m"

norm:
	norminette $(SOURCES)
