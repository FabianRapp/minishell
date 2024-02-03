/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 06:20:46 by frapp             #+#    #+#             */
/*   Updated: 2024/02/03 22:15:52 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
TODO:
	- close base fds
	- rework expansion
	- ft_exit completly bugged
	- redirs bugged with exit
	- parser return value must indicate a diffrence between syntax and malloc error
	- add early exits for sytax error in lexer and parser:
		-- if any subshell contains nothing or only whitespace its "bash: syntax error near unexpected token `)'" and no command even starts
	- implement pipes
	- implement other ft functions
	- implement here doc
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

	- change white space identification: not all whitespace is the same (mb dosnt matter since no multi line)

./test "asd >asd <sadad (asd) | >a <ad"
debug move_commands_inform : COMMAND

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

# define NEW_FILE_PERMISSIONS 0644

#define ARGS 2
# define IN 0
# define OUT 1

# define NOT_FINISHED 0
# define FALSE 1
# define SYNTAX_ERROR 2
# define EXIT_ERROR 3
# define FINISHED 4
# define EXIT 5

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
# include <fcntl.h>
# include <termios.h>
# include <signal.h>

// this projects headers
# include "libft.h"
# include "utils.h"
# include "lexer.h"
# include "tokens.h"
# include "parser.h"
# include "eval.h"
# include "signals.h"

typedef enum e_type		t_type;
typedef struct s_lexer	t_lexer;
typedef struct s_parser	t_parser;
typedef struct s_ast	t_ast;

// for the main proecess: through this struct all data
// is reachable for cleanup
typedef struct s_cleanup_data
{
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
	t_env_var	*vars;
	int			exit_status;
	bool		main_process;
	int			main_pid;
	int			size_name_fd[2];
	int			name_fd[2];
	int			size_val_fd[2];
	int			val_fd[2];
}	t_env;

typedef	struct s_redir	t_redir;
typedef	struct s_redir
{
	t_type			type;
	t_arg			*arg;
	t_redir			*next;
}	t_redir;

typedef struct s_child_data
{
	char		*path;
	char		*command_name;
	int			exit_status;
	bool		malloc_error;
	char		**argv;
}	t_child_data;



typedef struct s_ast
{
	t_type			type;
	t_token_list	*name;
	t_redir			*redir;
	t_arg			*arg;
	t_parser		*val;
	t_ast			*left;
	t_ast			*right;
	int				fd[2];
	int				exit_status_node;
	t_cleanup_data	*cleanup_data;
	t_env			*env;
	pid_t			id;
	int				exit_fd[2];
	int				*all_pids;
	int				base_fd[2];
}	t_ast;

// lexer
t_token		*next_new_token(t_lexer *lexer, bool *malloc_fail);
t_lexer		new_lexer(char *str);

// main
void	walk_ast(t_ast *ast);
void	run_node(t_ast *ast);
void	run_command_node(t_ast *ast);

bool	check_edgecases(t_ast *ast);

//redir
bool	resolve_redirs(t_ast *ast);
bool	reset_stdio(t_ast *ast);
bool	redir_stdio(t_ast *ast);

// debug lexer
void		print_token(t_token *token, t_parser *parser, int depth);
bool		test_lexer_manualy(char *str);

//env
bool	init_env(t_env *new_env, char **base_env);
void	free_env(t_env *env);
int		get_pid(void);
void	print_env(t_env *env);
t_env	clone_env(t_env *base);


void	ft_exit(t_ast *ast);

// utils
bool	my_free(void **ptr);
void	print_error(bool shell_name, char *command_name, char *arg, char *str);

//parser
typedef struct s_parser	t_parser;
typedef struct s_ast	t_ast;

t_ast	*parser(char *str);

#endif