/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:24:35 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/27 13:29:58 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* additional stuff:
- multi line for double quotes, single quotes and round brackets
- semicolon
- Real subshells like bash does
- env -i
- HERE-STR (<<<)
- our own get_pid function and $$ expansion

weird things that bash does that we do:
- if the HERE-DOC (<<) arg is in duble quotes the lines are expanded
	before written to a pipe
- HERE-DOC is executed halvly during parsing and half during redirection
- HERE-DOC gives waring when ctrl + D is pressed that shows the total lines
	(of the entire shell) read so far
- redirs can have two args: left and right. the left can be any open fd
	(ech 2>a redirs the error message to a)
- the right HERE-DOC arg is not expanded
- env vars with whitespace as redir args without double quotes leads to
	multiple arugments for a redir which results in an error
- if export is not successfull the env is not added but when export is
	ran without args the input is still displayed
- newline is not whitespace
- redirs on the left of a subshell are invalid while redirs on the right
	redir the entire subshell
*/
# ifdef __linux__
#  include <sys/types.h>
#  include <sys/wait.h>
# endif

/* *****************************    C LIBRARIES    ************************** */
# include <sys/stat.h>
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

/* *************************    PROJECTS'S HEADERS    ********************** */
# include "libft.h"
# include "lexer.h"
# include "parser.h"
# include "expansion.h"
# include "execution.h"

/* *************************    MACROS' DEFINITIONS    ********************* */
# ifndef TESTER
#  define TESTER 0
# endif

# ifndef SHELL_NAME
#  define SHELL_NAME "minishell\0"
# endif

# ifndef SHELL_PROMPT
#  define SHELL_PROMPT "minishell-$: \0"
# endif

# ifndef NEW_FILE_PERMISSIONS
#  define NEW_FILE_PERMISSIONS 0644
# endif

# ifndef INIT_VAL
#  define INIT_VAL -1
# endif

# ifndef DEFAULT_EXIT_STATUS
#  define DEFAULT_EXIT_STATUS -1
# endif

# ifndef ARGS
#  define ARGS 2
# endif

# ifndef READ
#  define READ 0
# endif

# ifndef WRITE
#  define WRITE 1
# endif

# ifndef STD_ERROR
#  define STD_ERROR 2
# endif

# ifndef LEAK_CHECK
#  define LEAK_CHECK 0
# endif

# define NAME 'n'
# define RECURSIVE_CALL 'r'

typedef enum e_result
{
	ERROR = false,
	SUCCESS = true,
}	t_result;

/******************************************************************************/
/* ******************            GENERAL STRUCTS:           ***************** */
/******************************************************************************/

typedef struct s_token_list				t_token_list;
typedef struct s_arg					t_arg;
typedef struct s_redir					t_redir;
typedef struct s_ast					t_ast;
typedef enum e_type						t_type;
typedef struct s_here_doc_child_data	t_here_doc_child_data;

typedef struct s_cleanup_data
{
	t_ast					*root;
	char					*input;
	struct s_shared_data	*shared_data;
}	t_cleanup_data;

typedef struct s_shared_data
{
	int					main_pid;
	bool				stop_execution;
	char				***envs;
	char				***env_exp;
	t_cleanup_data		*cleanup_data;
	struct sigaction	sig_set;
}	t_shared_data;

typedef struct s_ast
{
	t_type			type;
	t_token_list	*name;
	t_arg			*arg;
	t_redir			*redir;
	t_ast			*left;
	t_ast			*right;
	int				exit_status;
	t_shared_data	*shared_data;
	pid_t			pid;
	int				fd_to_close;
	int				fd_to_close_read;
	int				fd_to_close_write;
	bool			dont_run_buildins;
}	t_ast;

typedef struct s_fd_set
{
	int	base_fd;
	int	overload_with_fd;
	int	base_fd_backup;
}	t_fd_set;

void		add_global_data(t_ast *ast, t_shared_data *shared_data);

/******************************************************************************/
/* ******************               BUILTINS:               ***************** */
/******************************************************************************/
bool		ft_builtin_control(t_ast *ast);
int			ft_pwd(t_ast *ast);
int			ft_env(t_ast *ast);
void		ft_export(t_ast *ast, t_arg *cur_arg);
void		ft_unset(t_ast *ast);
void		ft_exit(t_ast *ast);
t_result	ft_echo(t_ast *ast, t_arg *cur_arg);
t_result	ft_cap_echo(t_ast *ast, t_arg *cur_arg);

/****************    cd and corresponding utils:   ******************/
int			ft_cd(t_ast *ast);

typedef struct s_cd_vars
{
	char	**steps;
	char	*cd_arg;
	int		i;
	char	pwd_before[PATH_MAX + 1];
	char	old_pwd[PATH_MAX + 1];
}	t_cd_vars;

typedef struct s_cd_step_data
{
	t_ast	*ast;
	char	*step;
	int		index;
	char	*cd_arg;
	bool	first;
	char	to_go[PATH_MAX + 1];
	char	before[PATH_MAX + 1];
	char	after[PATH_MAX + 1];
	char	old_pwd[PATH_MAX + 1];
}	t_cd_step_data;

t_result	check_path_len(t_ast *ast, char *path);
char		*get_parent_dir_path(void);
int			ft_update_dir_vars(t_ast *ast, char *before, char *after);
char		*init_ft_cd_step(t_ast *ast, char *step, int inde);

/******************************************************************************/
/* *****************               ENVIRONMENT:               *************** */
/******************************************************************************/

char		**ft_initialize_env(char **base_env);
char		*get_env_value(char **env, char *var_name,
				char *buffer, int buf_size);
char		**new_env_list_after_add(char *str_to_add, char **env, bool plus);
char		*get_env_var_name(char *line, bool plus);
char		**new_env_list_after_delete(char *var_to_rm, char **env_before);
void		ft_update_env(char *var_name, char *new_value, char **env);
char		***get_env_list(char ***set_new_env);

/******************************************************************************/
/* *********************              SIGNALS:             ****************** */
/******************************************************************************/

void		set_signals(void);
// t_result	set_ctrl_c(void);
// void		signal_handler_ctrl_c(int signal, siginfo_t *info, void *data);
// t_result	set_ctrl_slash(void);
bool		redisplay_prompt(bool set_state, bool new_state);
// void		init_terminal_settings(void); //? it doesn't seem to be used
// void		reset_terminal_settings(void);//? goes with the above uncalled
void		reset_signals(void);

/******************************************************************************/
/* *********************               UTILS:              ****************** */
/******************************************************************************/

typedef struct s_status_handler
{
	bool	set;
	int		val;
}	t_status_handler;
char		*ft_read_line(char *header);
t_result	set_ctrl_c_heredoc(void);
t_result	insert_whitespace_before(t_token_list **head);
t_result	insert_whitespace_end(t_token_list **list);
t_result	add_token_back(t_token_list **list, t_token *token);
t_token		*new_dummy_token(void);
void		free_token_list(t_token_list *list);
void		free_arg_list(t_arg *list);
void		free_ast(t_ast *ast);
t_result	wait_all_children(t_ast *ast);
void		free_token(t_token *token);
bool		ft_free(void **ptr);
int			ft_close(int *fd);
t_result	errno_to_result(void);
t_result	set_errno_as_exit(t_ast *ast, bool msg);
void		print_error(bool shell_name, char *command_name, char *arg,
				char *str);
void		print_error_weird_quotes(bool shell_name,
				char *command_name, char *arg, char *str);
bool		full_exit_status(bool set_full_exit);
void		set_last_exit(int exit_status);
char		*get_last_exit_str(void);
int			get_last_exit(void);
t_result	ft_cur_exit(t_ast *ast, int exit_value);
t_fd_set	*io_data(int flag, void *data);
t_result	redir_fds(void);
t_result	cleanup_fds(void);
t_fd_set	*get_fds(void);
bool		sub_shell_mode(int flag);
int			line_counter(void);
bool		is_command_block_terminator(t_type type);
bool		is_redir(t_type type);
bool		is_redir_arg_terminator(t_type type);
bool		is_word_terminator(t_type type);
bool		command_terminator(t_type type);
bool		is_termination_char(char c);
bool		is_wildcard_block_termination(char c);
bool		is_operator(t_type type);
char		*type_to_error(t_type type);
void		add_token_back_node(t_token_list **list, t_token_list *new_node);
void		add_token_node_front(t_token_list **head, t_token_list *new_list);
char		*ft_read_line(char *header);
int			name_len(char *str);
bool		here_doc_exit_state(bool change_state, bool new_state);
bool		ignore_empty_line(bool set_next_to_ignore);
bool		ft_iswhitespace_minishell(char c);
//DEBUGGING FUNCTIONS??

#endif	//MINISHELL_H
