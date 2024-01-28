/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 06:20:46 by frapp             #+#    #+#             */
/*   Updated: 2024/01/28 04:20:46 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
TODO:
	-redir env var expansion does not make sense, either needs additional pointer in structs or idk ($<varname> expands to multile redir eventhough it should be 1)
	- need to expand each redir one by one and create the files, because
		if an invalid redir appears the ones before allready created the files but the ones afet not
	- have some kind of error checking for unclosed quotes in init_parser()
	- need to lex '=' for ft_export()? (maybe just hardcode an export type duo to unique
		interaction of export with white space (does not accept any whitespace))
	- simplify lexer by using pointers instead of indexes
*/

/*
weird stuff to keep in mind

1.:
	-if an redir has more than 1 arg its:
		"bash: $<var name>: ambiguous redirect"
		this is caused if a variable is expanded as a redir arg without double quots when it incldes whitespace
2.:
	- exit does not exit if it is part of a pipe (only exits sub process)
	- exit prints an error for non integer args but still exits, unless:
	- if more than 0 or 1 arg is given does not exit
*/

#ifndef MINISHELL_H
# define MINISHELL_H

# define SHELL_NAME "minishell"

# define DEBUG 0

#define ARGS 1
#define IN 2
#define OUT 3


// libs
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <stdlib.h>


// this projects headers
# include "libft.h"
# include "utils.h"
# include "lexer.h"
# include "tokens.h"
# include "parser.h"
# include "eval.h"


typedef enum e_type		t_type;
typedef struct s_lexer	t_lexer;
typedef struct s_parser	t_parser;

// lexer
t_token		*next_new_token(t_lexer *lexer);
t_lexer		new_lexer(char *str);

// debug lexer
void		print_token(t_token *token, t_parser *parser, int depth);
bool		test_lexer_manualy(char *str);

// utils
bool	my_free(void **ptr);

//parser
typedef struct s_parser	t_parser;
typedef struct s_ast	t_ast;

t_ast	*parser(char *str);

#endif