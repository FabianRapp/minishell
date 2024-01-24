/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 08:52:07 by frapp             #+#    #+#             */
/*   Updated: 2024/01/24 22:28:04 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

#define DEBUG_EXTRACT_TOKENS 1

#define NAME 'n'
#define ARG 'a'
#define RECURSIVE_CALL 'r'

typedef enum e_operators_ast
{
	AST_PIPE,
	AST_OR,
	AST_AND,
	AST_COMMAND
}	t_operators_ast;

typedef struct s_parser	t_parser;

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

typedef	struct s_arg	t_arg;
typedef	struct s_arg
{
	t_type			type;
	t_token_list	*name;
	t_arg			*next;
}	t_arg;

typedef struct s_ast_command
{
	t_token_list	*name;
	t_arg			*redir_in;
	t_arg			*redir_out;
	t_arg			*args;
}	t_ast_command;

t_parser	*init_parser(char *str);
bool		insert_token(t_parser **parser, t_token *token);
t_parser	*link_parser(char *str);
bool		continue_parser(t_parser **parser);
void		jump_to_start(t_parser **parser);
void		free_token(t_token *token);
int			remove_parser_node(t_parser **node, bool free_tok);
bool		insert_token(t_parser **insert_after, t_token *insert_token);
void		free_parser_main(t_parser *parser);
bool		is_redir(t_type type);
bool		is_redir_arg_terminator(t_type type);
bool		is_operator(t_type type);
bool		command_name_terminator(t_type type);
bool		is_word_terminator(t_type type);
t_token		*peek_next_lexer_token(t_lexer *lexer);
t_type		peek_next_lexer_type(t_lexer *lexer);
bool		command_terminator(t_type type);

#endif

