CC=cc
FLAGS_NO_LEAK_CHECK = -fsanitize=address
CFLAGS=-Wall -Wextra -Werror  -g
# 
#-fsanitize=undefined 
#
LDFLAGS =
#
#
NAME=min
#
#
LIB_PARSER=parser
LIB_PARSER_NAME = parser.a
SOURCES= parser.c ../utils/utils1.c groups1.c ast.c \
parser_type_commands.c\
\
list_operations/utils1.c list_operations/add_new.c\
list_operations/move_nodes.c list_operations/remove_nodes.c

OBJECTS= $(SOURCES:.c=.o)

LIBFT=libft
LIBFT_NAME = libft.a
LIB_LEXER=lexer
LIB_LEXER_NAME = lexer.a
.PHONY: all clean fclean re libs $ $(LIB_PARSER)

build: CFLAGS += $(FLAGS_NO_LEAK_CHECK)
build: LDFLAGS += $(FLAGS_NO_LEAK_CHECK)
build: $(LIB_PARSER)

leaks: CFLAGS += -DLEAK_CHECK=1
leaks: $(LIB_PARSER)

$(LIB_PARSER): $(OBJECTS)
	@ar rcs $(LIB_PARSER_NAME) $(OBJECTS)
	@echo "\033[32mparser build\033[0m"

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $^

clean:
	@rm -f $(OBJECTS)
	@echo "\033[33mparser objects cleaned\033[0m"

fclean: clean
	@$(MAKE) -C .. fclean 

re: fclean $(LIB_PARSER)

norm:
	norminette $(SOURCES)

libs: $(LIBFT) $(LIB_LEXER)

$(LIBFT):
	@$(MAKE) -C .. $(LIBFT)
	@cp ../$(LIBFT_NAME) $(LIBFT_NAME)

$(LIB_LEXER):
	@$(MAKE) -C .. $(LIB_LEXER)
	@cp ../$(LIB_LEXER_NAME) $(LIB_LEXER_NAME)

