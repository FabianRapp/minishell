/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 06:20:46 by frapp             #+#    #+#             */
/*   Updated: 2024/01/26 23:35:53 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
TODO:

-- parser: something goes wrong if a command arg is followed by a sngle arg cosisiting of:
-word+literal or literal+word followed by something, then somthing is not printed
->build_ast needs refactor (ideally fully recursice)
-- util functions for AST replacements

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