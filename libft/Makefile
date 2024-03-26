
FLAGS_NO_LEAK_CHECK =
#  -fsanitize=undefined -fsanitize=address -g
CFLAGS=-Wall -Wextra -Werror

LDFLAGS =
# -fsanitize=undefined -fsanitize=address -g
#
NAME	=	libft.a
CFLAGS	=	-Wall -Wextra -Werror
CC		=	clang

GREEN	=	\033[0;32m
YELLOW	=	\033[33m
CYAN	=	\033[0;36m
CLEAR	=	\033[0m

OBJ_DIR	=	./obj/

SRCS	=	ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
			ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c \
			ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c \
			ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c \
			ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c \
			ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
			ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
			ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c \
			ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c \
			get_next_line_utils.c get_next_line.c ft_printf.c ft_printf_handle_c.c \
			ft_printf_handle_di.c ft_printf_handle_u.c ft_printf_handle_p.c \
			ft_printf_handle_percent.c ft_printf_handle_s.c ft_printf_handle_x.c \
			ft_printf_handle_xx.c ft_expo.c ft_realloc_copy_until_zeroed.c \
			ft_strarr_size.c ft_free_2darr.c ft_strjoin_free_s1.c ft_strjoin_free_both.c \
			ft_split_wildcards.c ft_split_fn.c ft_iswhitespace.c

OBJS	=	$(SRCS:%.c=$(OBJ_DIR)%.o)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "$(GREEN)libft build$(CLEAR)"

$(OBJ_DIR)%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(CYAN)libft objects cleaned$(CLEAR)"

fclean:
	@rm -f $(NAME)
	@rm -rf $(OBJ_DIR)
	@echo "$(CYAN)libft fclean$(CLEAR)"

re: fclean all

all: $(NAME)

flags: CFLAGS += -fsanitize=undefined -fsanitize=address -g
flags: LDFLAGS += -fsanitize=undefined -fsanitize=address -g
flags: $(NAME)

build: CFLAGS += $(FLAGS_NO_LEAK_CHECK)
build: LDFLAGS += $(FLAGS_NO_LEAK_CHECK)
build: $(NAME) $(OBJS)

leaks: CFLAGS += -DLEAK_CHECK=1
leaks: $(NAME) $(OBJS)

.PHONY: all clean fclean re build
