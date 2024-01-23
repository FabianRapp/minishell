CC=cc
CFLAGS=-Wall -Wextra -Werror 
#-fsanitize=address
# -g -fsanitize=undefined 
LDFLAGS = 
#-fsanitize=address
LIB_LEXER=lexer
LIB_LEXER_NAME = lexer.a
SOURCES= lexer/utils/debugging.c ../utils/utils1.c lexer/lexer_main.c lexer/utils/utils.c  \
 lexer/identifiers/ident_type1.c
OBJECTS= $(SOURCES:.c=.o)

NAME_TESTER=test
SOURCES_TESTER= tester/tester_main.c
OBJECTS_TESTER=$(SOURCES_TESTER:.c=.o)

LIBFT = libft
LIBFT_NAME = libft.a

.PHONY: all clean fclean re libs $ $(LIB_LEXER)


$(LIB_LEXER): build

build: $(OBJECTS)
	@ar rcs $(LIB_LEXER_NAME) $(OBJECTS)
	@echo "\033[32mlexer build\033[0m"

$(NAME_TESTER): libs $(OBJECTS_TESTER) build
	@$(CC) $(OBJECTS_TESTER) $(OBJECTS) $(LIBFT_NAME) $(LIB_LEXER_NAME) -o $(NAME_TESTER) $(CFLAGS) $(LDFLAGS)
	@echo "\033[32mlexer_tester build\033[0m"

re$(NAME_TESTER): fclean $(NAME_TESTER)

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $^

clean:
	@rm -f $(OBJECTS) $(OBJECTS_TESTER)
	@echo "\033[33mlexer objects cleaned\033[0m"

fclean: clean
	@$(MAKE) -C .. fclean

re: fclean $(LIB_LEXER)

norm:
	norminette $(SOURCES)

$(LIBFT):
	@$(MAKE) -C .. $(LIBFT)
	@cp ../$(LIBFT_NAME) $(LIBFT_NAME)

libs: $(LIBFT)
