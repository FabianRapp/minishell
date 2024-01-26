/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 06:20:46 by frapp             #+#    #+#             */
/*   Updated: 2024/01/26 03:31:44 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
TODO:

-- redir without argument causes segfault
-- util functions for AST replacements

-- redir in between commmand and args is either parsed wrong: type_args()
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

// this projects headers
# include "libft.h"
# include "utils.h"
# include "lexer.h"
# include "tokens.h"
# include "parser.h"

typedef enum e_type		t_type;
typedef struct s_lexer	t_lexer;
typedef struct s_parser	t_parser;

// lexer
t_token		*next_new_token(t_lexer *lexer);
t_lexer		new_lexer(char *str);

// debug lexer
void		print_token(t_token *token, t_parser *parser, int depth);
bool		test_lexer_manualy(char *str);


//parser
typedef struct s_parser	t_parser;
typedef struct s_ast	t_ast;

t_ast	*parser(char *str);

#endif