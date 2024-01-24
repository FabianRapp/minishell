/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:16:07 by frapp             #+#    #+#             */
/*   Updated: 2024/01/24 21:19:10 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENS_H
# define TOKENS_H

# include "lexer.h"

typedef struct s_lexer	t_lexer;

typedef enum e_type
{
	UNKNOWN = false, // the variable unknow in the token must be filled with the unknown char
	T_EOF,
	WHITE_SPACE,
	PIPE,// '|' might be bugged with external commands->figure out later
	OR, // '||' might be bugged with external commands->figure out later
	AND, // '&&'
	ENV_VAR,
	EXIT_STATUS_REQUEST,
	FT_BUILDIN, // reimplemented functions that are based of my source code
	WILDCARD,
	CTRL_C,
	CTRL_D,
	CTRL_BACKSLASH,
	LITERAL, // the tokens str varialbe must hold the correct string
	INTERPRETED, // the tokens str varialbe must hold the correct uninterpreted string
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HERE_DOC,
	SUBSHELL, // only for || and &&
	FLAG,
	WORD,
	COMMAND, // not identified by lexer. during execution the system path must be check for this on runtime
	ARGUMENT, // not identified by lexer
	REDIR_ARG,
}	t_type;


typedef struct s_token	t_token;
typedef struct s_token
{
	t_type		type;
	char		*str_data;
	char		unknown;
	char		*input_str;
	int			input_position;
}	t_token;

void		init_token(t_token *token, t_lexer *lexer);

#endif

