CC=cc
CFLAGS=-Wall -Wextra -Werror

GENERAL_SOURCES=lexer/utils/debugging.c ../utils/utils1.c

LIB_LEXER=lexer
LIB_LEXER_NAME=lexer.a
SOURCES=$(GENERAL_SOURCES) lexer/lexer_main.c lexer/utils/utils.c lexer/list_tokens.c \
 lexer/identifiers/ident_type1.c lexer/identifiers/ident_commands.c
OBJECTS= $(SOURCES:.c=.o)

NAME_TESTER=test
SOURCES_TESTER=$(GENERAL_SOURCES)  tester/tester_main.c
OBJECTS_TESTER=$(SOURCES_TESTER:.c=.o)

LIBFT_DIR = ../libft
LIBFT = libft
LIBFT_NAME = libft.a
LIBFT_PATH = $(LIBFT_DIR)/$(LIBFT_NAME)

LIBS =  $(LIBFT) $(LIB_LEXER)
LIBS_NAME = $(LIBFT_NAME) $(LIB_LEXER_NAME) 

.PHONY: all clean fclean re libs $(LIBFT) $(LIB_LEXER)

$(LIB_LEXER): build

libs: $(LIBS)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)
	@cp $(LIBFT_PATH) $(LIBFT_NAME)

build: $(LIBFT) $(OBJECTS)
	@ar rcs $(LIB_LEXER_NAME) $(OBJECTS)
	@echo "\033[32mlexer build\033[0m"

$(NAME_TESTER): $(LIBFT) $(LIB_LEXER) $(OBJECTS_TESTER)
	@$(CC) $(OBJECTS_TESTER) $(LIBFT_NAME) $(LIB_LEXER_NAME) -o $(NAME_TESTER) $(CFLAGS)
	@echo "\033[32mlexer_tester build\033[0m"

re$(NAME_TESTER): fclean $(NAME_TESTER)

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $^

clean:
	@rm -f $(OBJECTS) $(OBJECTS_TESTER)
	@echo "\033[33mlexer objects cleaned\033[0m"

fclean: clean
	@rm -f $(LIB_LEXER_NAME) $(LIBFT_NAME) $(NAME_TESTER) a.out
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "\033[33mlexer fclean\033[0m"

re: fclean $(LIB_LEXER)

norm:
	norminette $(SOURCES)
