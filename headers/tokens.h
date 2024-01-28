/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:16:07 by frapp             #+#    #+#             */
/*   Updated: 2024/01/28 03:26:00 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENS_H
# define TOKENS_H

# include "lexer.h"

typedef struct s_lexer	t_lexer;

typedef enum e_type
{
	UNKNOWN = false, // if any unknow token comes out of lexer something is wrong
	T_EOF,
	WHITE_SPACE, //removed in parser
	PIPE,
	OR,
	AND, // '&&'
	ENV_VAR,
	EXIT_STATUS_REQUEST,
	WILDCARD,
	VOID, // remove in parser
	INTERPRETED,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HERE_DOC,
	SUBSHELL, // only for || and &&
	WORD,
	COMMAND, // not identified by lexer->parser. during execution the system path must be check for this on runtime
	ARGUMENT, // not identified by lexer->parser
	REDIR_ARG,// not identified by lexer->parser
	DUMMY_COMMAND, // empty command but not a syntax error (redir only)
	LITERAL,
}	t_type;


typedef struct s_token	t_token;
typedef struct s_token
{
	t_type		type;
	char		*str_data;
	char		unknown;
	char		*input_str;
	int			input_position;
	char		*old_data;
}	t_token;

void		init_token(t_token *token, t_lexer *lexer);

#endif

