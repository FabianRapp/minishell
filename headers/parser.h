/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 08:52:07 by frapp             #+#    #+#             */
/*   Updated: 2024/01/23 20:35:58 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

typedef struct s_parser	t_parser;
typedef struct s_ast	t_ast;

typedef struct s_parser
{
	t_token		*token;
	t_type		type;
	t_parser	*next;
	t_parser	*arg;
}	t_parser;

typedef enum e_ast_node_type
{
	COMMAND_NODE,
	OPERATOR_NODE,
}	t_ast_node_type;

typedef struct	s_command
{
	t_type		type;
	int			pid_io[2];
	t_parser	*redir_in;
	t_parser	*redir_out;
	t_parser	*name;
	t_parser	*arg;
}	t_command;

typedef struct	s_operator
{
	t_type			type;
	t_parser		*data;
	t_ast			*left;
	t_ast			*right;
}	t_operator;

typedef	union u_ast_data
{
	struct s_command	command;
	struct s_operator	operator;
}	t_ast_data;

typedef struct s_ast
{
	t_ast_node_type	type;
	t_ast_data		data;
}	t_ast;



t_parser	*init_parser(char *str);
bool		insert_token(t_parser **parser, t_token *token);
t_parser	*link_parser(char *str);
bool		continue_parser(t_parser **parser);
void		jump_to_start(t_parser **parser);
void		free_token(t_token *token);
int			remove_parser_node(t_parser **node);
bool		insert_token(t_parser **insert_after, t_token *insert_token);
void		free_parser_main(t_parser *parser);
bool		is_redir(t_type type);
bool		is_redir_arg_terminator(t_type type);
bool		is_operator(t_type type);

#endif
