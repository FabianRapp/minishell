/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:24:35 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/27 00:34:02 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

/******************************************************************************/
/* *********************              LEXER:               ****************** */
/******************************************************************************/

typedef enum e_type
{
	UNKNOWN = false,
	T_EOF,
	WHITE_SPACE,
	PIPE,
	OR,
	AND,
	ENV_VAR,
	EXIT_STATUS_REQUEST,
	PID_REQUEST,
	WILDCARD,
	VOID,
	//INTERPRETED,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_IN_OUT,
	HERE_DOC,
	SUBSHELL,
	WORD,
	COMMAND,
	ARGUMENT,
	REDIR_ARG,
	DUMMY_COMMAND,
	LITERAL,
	SEMICOL,
	HERE_STR,
}	t_type;

typedef enum e_result	t_result;
typedef enum e_type		t_type;

typedef struct s_token
{
	t_type		type;
	char		*str_data;
	char		unknown;
	char		*input_str;
	int			input_position;
	char		*left_redir_arg;
	bool		here_doc_arg_literal;
	char		*old_data;
}	t_token;

typedef struct s_lexer
{
	char	*str;
	int		position;
	int		read_position;
	char	cur_char;
	char	last_char;
}	t_lexer;

t_result	dollar_lexing(t_lexer *lexer, t_token *token);
char		*get_potential_fd(t_lexer *lexer);
char		*check_limis_potential_fd(char *left_redir_arg,
				t_lexer *lexer, t_lexer lexer_backup);
void		print_error_redir_arg(t_lexer *lexer);
t_result	redir_type(t_lexer *lexer, t_token *token, bool recursive_call);
void		basic_sign_type(t_lexer *lexer, t_token *token);
t_result	literal_type(t_lexer *lexer, t_token *token);
t_result	interpreted_type(t_lexer *lexer, t_token *token);
t_result	ident_wildcard_literals(t_lexer *lexer,
				t_token *token, bool skip_next_term);
t_token		*next_new_token(t_lexer *lexer, bool recursive_call);
void		read_char(t_lexer *lexer);
bool		is_redir_terminator_char(char c);
void		lexer_error(t_token *token);
t_lexer		new_lexer(char *str);
t_result	subshell_type(t_lexer *lexer, t_token *token);

#endif	//LEXER_H
