
CC=cc
CFLAGS=-Wall -Wextra -Werror -fsanitize=address
# -g -fsanitize=undefined 
LDFLAGS = -fsanitize=address
LIB_PARSER=parser
LIB_PARSER_NAME = parser.a
SOURCES= list_tokens.c parser.c
OBJECTS= $(SOURCES:.c=.o)

NAME_TESTER=test
SOURCES_TESTER= test.c
OBJECTS_TESTER=$(SOURCES_TESTER:.c=.o)

LIBFT_NAME = libft.a
LIB_LEXER_NAME = lexer.a
.PHONY: all clean fclean re libs $ $(LIB_PARSER)

$(LIB_PARSER): build

build: $(OBJECTS)
	@ar rcs $(LIB_PARSER_NAME) $(OBJECTS)
	@echo "\033[32mparser build\033[0m"

$(NAME_TESTER): libs $(LIB_PARSER) $(OBJECTS_TESTER) $(OBJECTS)
	@$(CC) $(OBJECTS_TESTER) $(OBJECTS) $(LIBFT_NAME) $(LIB_PARSER_NAME) -o $(NAME_TESTER) $(CFLAGS) $(LDFLAGS)
	@echo "\033[32mparser_tester build\033[0m"

re$(NAME_TESTER): fclean $(NAME_TESTER)

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $^

clean:
	@rm -f $(OBJECTS) $(OBJECTS_TESTER)
	@echo "\033[33mparser objects cleaned\033[0m"

fclean: clean
	@$(MAKE) -C .. fclean
	@rm -f test 

re: fclean $(LIB_PARSER)

norm:
	norminette $(SOURCES)

$(LIBFT_NAME):
	@$(MAKE) -C .. $(LIBFT_NAME)
	@cp ../$(LIBFT_NAME) $(LIBFT_NAME)

$(LIB_LEXER_NAME):
	@$(MAKE) -C .. $(LIB_LEXER_NAME)
	@cp ../$(LIB_LEXER_NAME) $(LIB_LEXER_NAME)

libs: $(LIBFT_NAME) $(LIB_LEXER_NAME)

