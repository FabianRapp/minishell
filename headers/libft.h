/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 22:22:26 by frapp             #+#    #+#             */
/*   Updated: 2024/01/21 16:12:28 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>

//part 1
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
size_t	ft_strlen(const char *s);
size_t	ft_strnlen(const char *s, size_t max);
void	*ft_memset(void *b, int c, size_t len);
void	ft_bzero(void *s, size_t n);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_memmove(void *dst, const void *src, size_t len);
size_t	ft_strlcpy(char *dst, const char *src,
			size_t dstsize);
size_t	ft_strlcat(char *dst, const char *src,
			size_t dstsize);
int		ft_toupper(int c);
int		ft_tolower(int c);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
int		ft_atoi(const char *str);
void	*ft_calloc(size_t count, size_t size);
char	*ft_strdup(const char *s1);
char	*ft_strndup(const char *s1, size_t max_size);

//part 2
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
void	ft_strjoin_inplace(char **s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
char	*ft_itoa(int n);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);

char	*get_next_line(int fd);
int		ft_printf(const char *format_str, ...);

int		ft_strcmp(const char *s1, const char *s2);
int		ft_expo(int base, int expo);
int		ft_iswhitespace(char c);
//bonus
//struct	s_list;
//typedef struct s_list	t_list;

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}t_list;

t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *new);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void*));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10000
# endif

typedef struct s_file	t_file;

struct s_file
{
	char	buffer[BUFFER_SIZE + 5];
	char	buffer_copy[BUFFER_SIZE + 5];
	char	*output;
	int		fd;
	int		buffer1_chars;
	int		output_size;
	t_file	*first_file;
	t_file	*next_file;
	int		cur_all_c;
	int		buffer1_i;
	int		return_now;
};

char	*cleanup_libft(t_file *current_file, t_file *first_file);
t_file	*get_current_file(int fd, t_file *first_file);
int		my_str_cpy(char *dest, char *src, char *src2);
void	sort_buffer(t_file *current_file);
char	*append_buffer(t_file *current_file);
char	*reading(t_file	*current_file);
char	*shrink_out(int fd, t_file *first_file);

#endif