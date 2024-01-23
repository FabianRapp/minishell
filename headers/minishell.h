/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 06:20:46 by frapp             #+#    #+#             */
/*   Updated: 2024/01/23 18:52:31 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
TODO:
-$123 is unkown type and buggy ($is a terminator)
	bash-3.2$ echo "$123"
	23
-need to parse '=' diffrent for ft_export()
-simplify lexer by using pointers instead of indexes
-maybe add a type to token (like a bool for command terminator)
-if the FT_BUILDIN type stays it needs capital char handleing (eChO is vlaid..)
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

typedef	enum e_redir	t_redir;
typedef enum e_type		t_type;
typedef struct s_lexer	t_lexer;
typedef struct s_parser	t_parser;

// lexer
t_token		*next_new_token(t_lexer *lexer);
t_lexer		new_lexer(char *str);

// debug lexer
void		print_token(t_token *token, t_parser *parser);
bool		test_lexer_manualy(char *str);


//parser
typedef struct s_parser	t_parser;
t_parser	*parser(char *str);

#endif