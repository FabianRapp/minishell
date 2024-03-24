/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 08:52:07 by frapp             #+#    #+#             */
/*   Updated: 2024/03/24 02:30:59 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

#define NAME 'n'
#define RECURSIVE_CALL 'r'

typedef struct s_parser	t_parser;
typedef struct s_cleanup_data	t_cleanup_data;

typedef struct s_parser
{
	t_token		*token;
	t_type		p_type;
	t_parser	*next;
	t_parser	*arg;
	t_parser	*rest_name;
}	t_parser;

// simplified node of t_parser list
typedef	struct s_token_list	t_token_list;
typedef	struct s_token_list
{
	t_token_list	*next;
	int				arr[1000];
	t_token			*token;
}	t_token_list;

typedef struct s_ast	t_ast;
typedef struct s_left_right_parsers
{
	t_parser	*left;
	t_parser	*right;
}	t_left_right_parsers;

t_ast	*parser(char *str);

// AST

t_parser				*find_highest_operator(t_parser *parser);
t_parser				*remove_back(t_parser *cut_location); //!something is red for me here!
t_left_right_parsers	split_parser(t_parser *split_location);
t_ast 					*build_ast(t_parser *parser);
void					free_ast(t_ast *ast);

#endif
