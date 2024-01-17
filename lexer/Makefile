CC=cc
CFLAGS=-Wall -Wextra -Werror

GENERAL_INCLUDES=-I../main_headers -I../utils -I../libft
GENERAL_SOURCES=

INCLUDES=$(GENERAL_INCLUDES) -I./lexer -I./tokens
LIB_LEXER=lexer.a
SOURCES=$(GENERAL_SOURCES) lexer/lexer_main.c lexer/utils/utils.c tokens/tokens.c
OBJECTS= $(SOURCES:.c=.o)

INCLUDES_TESTER=$(INCLUDES) -I./tester
NAME_TESTER=test
SOURCES_TESTER=tester/tester_main.c
OBJECTS_TESTER=$(SOURCES_TESTER:.c=.o)

LIBFT_DIR = ../libft
LIBFT = libft.a
LIBFT_PATH = $(LIBFT_DIR)/$(LIBFT)


.PHONY: all clean fclean re clean2

all:$(LIB_LEXER)

$(LIB_LEXER): build

#$(INCLUDES) $(LIBFT) $(CFLAGS)
build: $(LIBFT) $(OBJECTS)
	@ar rcs $(LIB_LEXER) $(OBJECTS)
	@echo "\033[32mBuild complete.\033[0m"

$(NAME_TESTER): $(LIBFT) $(LIB_LEXER) $(OBJECTS_TESTER)
	$(CC) $(INCLUDES_TESTER) $(OBJECTS_TESTER) $(LIBFT) $(LIB_LEXER) -o $(NAME_TESTER) $(CFLAGS)
	@echo "\033[32mBuild complete.\033[0m"

re$(NAME_TESTER): fclean $(LIBFT) $(LIB_LEXER) $(OBJECTS_TESTER)
	$(CC) $(INCLUDES_TESTER) $(OBJECTS_TESTER) $(LIBFT) $(LIB_LEXER) -o $(NAME_TESTER) $(CFLAGS)
	@echo "\033[32mBuild complete.\033[0m"

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)
	@cp $(LIBFT_PATH) $(LIBFT)

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) $(INCLUDES_TESTER) -o $@ -c $^

clean:
	@rm -f $(OBJECTS) $(OBJECTS_TESTER)
	@echo "\033[33mObject files removed.\033[0m"

fclean: clean
	@rm -f $(LIB_LEXER) $(LIBFT) $(NAME_TESTER) a.out
	@echo "\033[33mExecutable removed.\033[0m"

re: fclean all

clean2:
	@rm -f $(OBJECTS)
	@echo "\033[33mObject files removed.\033[0m"

norm:
	norminette $(SOURCES)
