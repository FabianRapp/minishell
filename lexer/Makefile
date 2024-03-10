FLAGS_NO_LEAK_CHECK =
#-fsanitize=undefined -fsanitize=address
#
CFLAGS=-Wall -Wextra -Werror
#-g
#
# 
# 
#
LDFLAGS =
#
#
LIB_LEXER=lexer
LIB_LEXER_NAME = lexer.a
SOURCES= ../utils/utils1.c ../utils/debugging.c lexer_main.c utils/utils.c  \
 identifiers/ident_type.c identifiers/dollar_sign.c identifiers/subshell.c \
 identifiers/ident_redir.c\
  ../utils/utils1.c
OBJECTS= $(SOURCES:.c=.o)

NAME_TESTER=test
SOURCES_TESTER= tester/tester_main.c
OBJECTS_TESTER=$(SOURCES_TESTER:.c=.o)

LIBFT = libft
LIBFT_NAME = libft.a

.PHONY: all clean fclean re libs $ $(LIB_LEXER)


build: CFLAGS += $(FLAGS_NO_LEAK_CHECK)
build: LDFLAGS += $(FLAGS_NO_LEAK_CHECK)
build: $(OBJECTS)
build: $(LIB_LEXER)
$(LIB_LEXER): $(OBJECTS)
	@ar rcs $(LIB_LEXER_NAME) $(OBJECTS)
	@echo "\033[32mlexer build\033[0m"


leaks: CFLAGS += -DLEAK_CHECK=1
leaks: $(OBJECTS)
leaks: $(LIB_LEXER)

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
