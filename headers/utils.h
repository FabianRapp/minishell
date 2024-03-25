/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 06:23:25 by frapp             #+#    #+#             */
/*   Updated: 2024/03/25 00:27:35 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "minishell.h"

typedef enum e_type				t_type;
typedef enum e_result			t_result;

bool			is_termination_char(char c);
bool			is_wildcard_block_termination(char c);

#define RESET_COLOR "\x1b[0m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define WHITE "\x1b[37m"

typedef struct s_ast	t_ast;
typedef struct s_token	t_token;

typedef struct s_status_handler
{
	bool	set;
	int		val;
}	t_status_handler;

char			*type_to_error(t_type type);
int				ft_close(int *fd);
bool			is_operator(t_type type);
t_result		errno_to_result(void);
t_result		set_errno_as_exit(t_ast *ast, bool msg);

// debug
char			*type_to_str(t_type type);
void			print_indent(int depth, bool left);
void			print_colored(const char *text, int color_index);
void			print_new_indent(int depth, bool left);
void			print_indent_arg(int depth);
void			check_fds(void); // for debbuging (needs to be removed, un allowed fn)

typedef	struct s_token_list	t_token_list;
typedef	struct s_arg		t_arg;
typedef	struct s_parser		t_parser;
typedef struct s_ast		t_ast;
typedef enum e_result		t_result;

// groups.c
bool			is_command_block_terminator(t_type type);
bool			is_redir(t_type type);
bool			is_redir_arg_terminator(t_type type);
bool			is_word_terminator(t_type type);
bool			command_terminator(t_type type);

t_result		insert_whitespace_end(t_token_list **list);
t_result		add_token_back(t_token_list **list, t_token *token);
void			add_arg_front(t_arg **head, t_arg *new);
void			add_token_back_node(t_token_list **list, t_token_list *new_node);
void			add_token_node_front(t_token_list **head, t_token_list *new);
t_result		insert_whitespace_before(t_token_list **head);
void			free_token_list(t_token_list *list);
void			free_arg_list(t_arg *list);

t_token			*new_dummy_token(void);
void			free_token(t_token *token);
void			print_ast(t_ast *ast);
t_result		wait_all_children(t_ast *ast);

// char			*expand_dollar(char *dollar_str, int *index);

void			print_token_list(t_token_list *token_node, int level);
void			print_arg_list(t_arg *arg, int level, bool left);
void			print_parser(t_parser *parser, int tree_level);

char			*ft_read_line(char *header);
int				line_counter(void);
int				count_open_fds(void);
char			*extract_command_name(char *path);
// used in lexer and expanding env cars (repl?)
int				name_len(char *str);

#endif