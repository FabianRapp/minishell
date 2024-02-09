/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 06:20:46 by frapp             #+#    #+#             */
/*   Updated: 2024/02/09 19:14:11 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
TODO:
	- last exit setter/getter
	- env vars rework
	- finish expansion rework
	- exit behivior with two numeric args (stops curent command execution but does not exit)
	- parser must print syntax error and return NULL in case of error
	- add early exits for sytax error in lexer and parser:
		-- if any subshell contains nothing or only whitespace its "bash: syntax error near unexpected token `)'" and no command even starts
	- implement other ft functions
	- implement here doc
	- update path functions to use new error print fn
	- add error handeling to lexer and parser
	- have some kind of error checking for unclosed quotes in init_parser()
	- add 
	- change white space identification: not all whitespace is the same (mb dosnt matter since no multi line)
minishell-$: (asd) (asd)
minishell: asd: command not found
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

# define DEFAULT_EXIT_STATUS -1

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
	pid_t			pid;
	int				*all_pids;
	int				base_fd[2];
	char			**envs;
}	t_ast;

// lexer
t_token		*next_new_token(t_lexer *lexer, bool *malloc_fail);
t_lexer		new_lexer(char *str);

// main
void	run_node(t_ast *ast);
void	run_command_node(t_ast *ast);

bool	check_edgecases(t_ast *ast);
void	add_global_data(t_ast *ast, t_env *env, char **envs);

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

// utils
bool	my_free(void **ptr);
void	print_error(bool shell_name, char *command_name, char *arg, char *str);

//parser
typedef struct s_parser	t_parser;
typedef struct s_ast	t_ast;

t_ast	*parser(char *str);

#endif



// (echo D) || (echo E && echo F | echo G) && (echo H || echo I) | (echo J && echo K) || echo L && (echo M || echo N)


//(echo A && (echo B | echo C) && echo D) || (echo E && echo F | echo G) && (echo H || echo I) | (echo J && echo K) || echo L && (echo M || echo N)

/*

(echo A && (echo B | echo C) && echo D) || (echo E && echo F | echo G) && (echo H || echo I) | (echo J && echo K) || echo L && (echo M || echo N)


(echo H) | (echo J && echo K) || echo L			H J K L		(J K)
echo H | (echo J && echo K) || echo L			J K L		

echo H | (echo J && echo K) || echo L

(echo H) | (echo J && echo K)


(echo J && echo K) || echo L

(echo J) || echo L


(echo A && (echo B | echo C) && echo D) || (echo E && echo F | echo G) && (echo H || echo I) | (echo J && echo K) || echo L


(echo A && (echo B | echo C) && echo D) || (echo E && echo F | echo G) && (eco H || echo I) | (eho J && echo K) || echo L


(echo A && (echo B | echo C) && echo D) || (echo E && echo F | echo G) && (eco H) | (echo J && echo K) || echo L

(echo A && (echo B || echo C) && (echo D | (echo E && echo F))) || (echo G && (echo H | echo I)) && ((echo J || echo K) && echo L) | (echo M && (echo N || echo O)) && (echo P || (echo Q && echo R)) || echo S && ((echo T && echo U) || echo V) | (echo W || echo X) && echo Y || (echo Z && echo AA) || echo AB && (echo AC || echo AD) && (echo AE | echo AF && echo AG)

(echo H || echo I) | (echo J && echo K) || echo L && (echo M || echo N)



echo H | (echo J) || echo L


echo P || (echo Q && echo R)

bash-3.2$ (echo A && (echo B || echo C) && (echo D | (echo E && echo F))) || (echo G && (echo H | echo I)) && ((echo J || echo K) && echo L) | (echo M && (echo N || echo & (echo P || (echo Q && echo R)) || echo S && ((echo T && echo U) || echo V) | (echo W || echo X) && echo Y || (echo Z && echo AA) || echo AB && (echo AC || echo AD) && (AE | echo AF && echo AG)
A
B
E
F
M
N
P
W
Y
AC
AF
AG
bash-3.2$ ./minishell
minishell-$: (echo A && (echo B || echo C) && (echo D | (echo E && echo F))) || (echo G && (echo H | echo I)) && ((echo J || echo K) && echo L) | (echo M && (echo N || echo O)) && (echo P || (echo Q && echo R)) || echo S && ((echo T && echo U) || echo V) | (echo W || echo X) && echo Y || (echo Z && echo AA) || echo AB && (echo AC || echo AD) && (echo AE | echo AF && echo AG)
A
B
E
F
M
N
P
R
W
Y
AC
AF
AG



(echo A && (B || echo C) && (D | (echo E && F)))


(D | (echo E && F)) && echo G

(echo E && F) && echo G





(echo H | I) && (echo N)


*/


