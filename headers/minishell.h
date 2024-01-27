/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 06:20:46 by frapp             #+#    #+#             */
/*   Updated: 2024/01/27 05:17:23 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
TODO:
-- add literals back in: literals are not split and words must be split at white space (needed for edge cases)
->many additonal contions
->words in arguments expand to the same argument; for commands the first is the command and afterwords args?
->reason for ambiguous redirect(below), duo to multiple args for redir?
->when a command start to execute no word should be left, only literals
-- repl/utils/expand_strs1.c: expand_token_list() make env var handling correct (echo $PATH (no quotes))

-need to parse '=' for ft_export()?
-simplify lexer by using pointers instead of indexes
*/

/*
weird stuff to keep in mind

1.:
bash-3.2$ echo $test
a b
bash-3.2$ $test
bash: a: command not found
bash-3.2$ >$test
bash: $test: ambiguous redirect

*/

#ifndef MINISHELL_H
# define MINISHELL_H

# define DEBUG 0

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