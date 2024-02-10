/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 08:52:07 by frapp             #+#    #+#             */
/*   Updated: 2024/02/10 19:44:04 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

#define DEBUG_EXTRACT_TOKENS 0

#define NAME 'n'
#define ARG 'a'
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


typedef	struct s_token_list	t_token_list;
// simplified node of t_parser list
typedef	struct s_token_list
{
	
	t_token			*token;
	t_token_list	*next;
}	t_token_list;




typedef struct s_ast	t_ast;


typedef struct s_left_right_parsers
{
	t_parser	*left;
	t_parser	*right;
}	t_left_right_parsers;


t_ast	*parser(char *str);

t_parser	*init_parser(char *str);
bool		insert_token(t_parser **parser, t_token *token);
t_parser	*link_parser(char *str);
void		jump_to_start(t_parser **parser);
void		free_token(t_token *token);
int			remove_parser_node(t_parser **node, bool free_tok);
void		free_parser_main(t_parser *parser, bool free_tokens);
bool		is_redir(t_type type);
bool		is_redir_arg_terminator(t_type type);
bool		is_operator(t_type type);
bool		command_name_terminator(t_type type);
bool		is_word_terminator(t_type type);
bool		command_terminator(t_type type);
void		free_ncircular_parser(t_parser *parser, bool free_tok);


// AST

t_parser				*last_parser(t_parser *parser);
t_parser				*find_highest_operator(t_parser *parser);
t_parser				*remove_back(t_parser *cut_location);
t_left_right_parsers	split_parser(t_parser *split_location);
t_ast 					*build_ast(t_parser *parser);
void					free_ast(t_ast *ast);



#endif
