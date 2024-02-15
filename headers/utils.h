/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 06:23:25 by frapp             #+#    #+#             */
/*   Updated: 2024/02/15 05:16:32 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "minishell.h"

typedef enum e_type	t_type;

void			cleanup(char *location);
bool			is_termination_char(char c);

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

char	*type_to_str(t_type tokenType);

bool		is_operator(t_type type);

// debug
char			*type_to_str_type(t_type tokenType);
void			print_indent(int depth, bool left);
void			print_colored(const char *text, int color_index);
void			print_new_indent(int depth, bool left);
void			print_indent_arg(int depth);

typedef	struct s_token_list	t_token_list;
typedef	struct s_arg		t_arg;
typedef	struct s_parser		t_parser;
typedef struct s_ast		t_ast;
typedef enum e_result		t_result;


t_token		*new_dummy_token(void);
void		free_token(t_token *token);
void		print_ast(t_ast *ast);
t_result	wait_all_children(void);


void			print_token_list(t_token_list *token_node, int level);
void			print_arg_list(t_arg *arg, int level, bool left);
void			print_parser(t_parser *parser, int tree_level);



// used in lexer and expanding env cars (repl?)
int				name_len(char *str);

#endif