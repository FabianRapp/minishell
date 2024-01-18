/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:16:07 by frapp             #+#    #+#             */
/*   Updated: 2024/01/18 11:35:05 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENS_H
# define TOKENS_H

typedef	enum e_redir
{
	REDIR_IN = 0, // <
	REDIR_OUT = 1, // >
	REDIR_APPEND = 2, // <<
	REDIR_HEREDOC = 3 // >>
}	t_redir;

// TODO:
// Missing: Normal functions
typedef enum e_type
{
	UNKNOWN = false, // the variable unknow in the token must be filled with the unknown char
	T_EOF,
	WHITE_SPACE,
	PIPE,// '|' might be bugged with externam commands->figure out later
	OR, // '||' might be bugged with externam commands->figure out later
	AND, // '&&'
	ENV_VAR,
	EXIT_STATUS_REQUEST,
	FT_BUILDIN, // the str varialbe in the token must be filled with the function name and in case of echo with the optional argument '-n' if it was given
	WILDCARD,
	CTRL_C,
	CTRL_D,
	CTRL_BACKSLASH,
	LITERAL, // the tokens str varialbe must hold the correct string
	INTERPRETED, // the tokens str varialbe must hold the correct uninterpreted string
	REDIR,
	SUBSHELL,
	FLAG,
	WORD,
	//INTEGER,
}	t_type;

typedef struct s_token	t_token;
typedef struct s_token
{
	t_type		type;
	int64_t		int_val;
	char		*str;
	char		unknown;
	t_token		*last;
	t_token		*next;
}	t_token;

void		zero_token(t_token *token);

#endif

