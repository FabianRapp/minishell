/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 06:20:46 by frapp             #+#    #+#             */
/*   Updated: 2024/01/31 13:50:44 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
TODO:
	- add early exits for sytax error in lexer and parser:
		-- if any subshell contains nothing or only whitespace its "bash: syntax error near unexpected token `)'" and no command even starts
	- implement pipes
	- implement other ft functions
	- implement redirs
	- update path functions to use new error print fn
	- add error handeling to lexer and parser
	- maybe need to keep track if the main process or a child is running a command for exit()?
	- add current working dir to path
	-redir env var expansion does not make sense, either needs additional pointer in structs or idk ($<varname> expands to multile redir eventhough it should be 1)
	- need to expand each redir one by one and create the files, because
		if an invalid redir appears the ones before allready created the files but the ones afet not
	- have some kind of error checking for unclosed quotes in init_parser()
	- need to lex '=' for ft_export()? (maybe just hardcode an export type duo to unique
		interaction of export with white space (does not accept any whitespace))
	- simplify lexer by using pointers instead of indexes

	- change white space identification: not all whitespace is the same (mb dosnt matter since no multi line)

	lexer:
	bash-3.2$ "asd
	> bash: unexpected EOF while looking for matching `"'
	bash: syntax error: unexpected end of file
	bash-3.2$ 'asd
	> bash: unexpected EOF while looking for matching `''
	bash: syntax error: unexpected end of file
	bash-3.2$ (asd
	> bash: syntax error: unexpected end of file

*/

/*
weird stuff to keep in mind about the code
- printf will cause weird bugs, use ft_printf ft_fprintf, print_error or write

weird stuff to keep in mind about bash

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

# define SHELL_NAME "minishell\0"
# define SHELL_PROMPT "minishell: \0"

# define DEBUG 0

#define ARGS 1
#define IN 2
#define OUT 3

# define NOT_FINISHED 0
# define FALSE 1
# define SYNTAX_ERROR 2
# define EXIT_ERROR 3
# define FINISHED 4


// libs
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <dirent.h>

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
typedef struct s_ast	t_ast;

// for the main proecess: through this struct all data
// is reachable for cleanup
typedef struct s_cleanup_data
{
	bool	main_process;
	t_ast	*root;
	char	*input;
}	t_cleanup_data;

typedef struct s_env_var
{
	char	*name;
	char	*val;
}	t_env_var;

typedef struct s_env
{
	t_env_var	*extra_vars;
	t_env_var	*banned_vars;
	char		*last_dir;
	int			*last_exit_status;
	int			pid;
}	t_env;

typedef struct s_ast
{
	t_type			type;
	t_token_list	*name;
	t_arg			*redir_in;
	t_arg			*redir_out;
	t_arg			*arg;
	t_parser		*val;
	t_ast			*left;
	t_ast			*right;
	int				exit_status;
	int				info;
	bool			main_process;
	t_cleanup_data	*cleanup_data;
	t_env			*env;
}	t_ast;

// lexer
t_token		*next_new_token(t_lexer *lexer, bool *malloc_fail);
t_lexer		new_lexer(char *str);

// main
void	walk_ast(t_ast *ast, t_cleanup_data *cleanup_data, bool main_process);
void	run_command_node(t_ast *ast);

// debug lexer
void		print_token(t_token *token, t_parser *parser, int depth);
bool		test_lexer_manualy(char *str);

//env
void	init_env(t_env *new_env, t_ast *ast, t_ast *sub_process);
void	free_env(t_env *env);
int		get_pid(void);

// utils
bool	my_free(void **ptr);
void	print_error(bool shell_name, char *command_name, char *arg, char *str);

//parser
typedef struct s_parser	t_parser;
typedef struct s_ast	t_ast;

t_ast	*parser(char *str);

#endif