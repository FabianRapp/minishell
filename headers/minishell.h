/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 06:20:46 by frapp             #+#    #+#             */
/*   Updated: 2024/03/26 03:21:22 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
             TOTAL TEST COUNT: 994  TESTS PASSED: 978  LEAKING: 0
                     STD_OUT: 10  STD_ERR: 1  EXIT_CODE: 6
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 17
                                     ✅ 2965
*/

#ifndef MINISHELL_H
# define MINISHELL_H

# ifdef __linux__
#  include <sys/types.h>
#  include <sys/wait.h>
# endif

// libs
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

// this project's headers
# include "libft.h"
# include "utils.h"
# include "lexer.h"
# include "tokens.h"
# include "parser.h"

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

typedef enum e_result
{
	ERROR = false,
	SUCCESS = true,
}	t_result;

typedef enum e_type		t_type;
typedef struct s_lexer	t_lexer;
typedef struct s_parser	t_parser;
typedef struct s_ast	t_ast;

typedef struct s_arg
{
	t_type			type;
	t_token_list	*name;
	struct s_arg	*next;
}	t_arg;

typedef struct s_redir
{
	t_type			type;
	t_arg			*arg;
	struct s_redir	*next;
	int				left_redir_arg;
	char			*token_str_data;
	bool			here_doc_literal;
}	t_redir;

typedef struct s_child_data
{
	char		*path;
	char		*command_name;
	char		**argv;
}	t_child_data;

typedef struct s_fd_set
{
	int	base_fd;
	int	overload_with_fd;
	int	base_fd_backup;
}	t_fd_set;

// for the main proecess: through this struct all data
// is reachable for cleanup
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

typedef struct s_path
{
	char	*all_paths;
	char	*cur_path;
	int		position;
	int		read_postion;
	char	*command_name;
	t_ast	*ast;
}	t_path;

typedef struct s_pipe_data
{
	int			pipe_fd[2];
	int			base_write;
	int			base_read;
	int			left_pid;
	t_ast		*ast;
}	t_pipe_data;

// lexer
t_token		*next_new_token(t_lexer *lexer, bool recursuve_call);
t_lexer		new_lexer(char *str);

// main
void		run_node(t_ast *ast);
int			run_command_node(t_ast *ast);

// execution/run_command.c
int			run_command_node(t_ast *ast);

bool		check_edgecases(t_ast *ast);
void		add_global_data(t_ast *ast, t_shared_data *env);

//redir
t_result	resolve_redirs(t_ast *ast);

// debug lexer
void		print_token(t_token *token, t_parser *parser, int depth);
bool		test_lexer_manualy(char *str);

//from eval.h:
t_result	expansion(t_ast *ast);
char		*find_path(t_ast *ast, char *command_name, char *path_env);
t_result	init_path_object(t_ast *ast, char *command_name,
				t_path *path_ob, char *path_var);
char		*handle_shell_fn(char *name);
char		*handle_absolute_path(char *path);
bool		next_path(t_path *path_ob);

// input_exit.c
t_ast		*get_input(t_cleanup_data *cleanup_data);
void		main_exit(t_cleanup_data *data, bool full_exit, bool ft_exit_call);
t_ast		*handle_manunal_input(char **av, t_cleanup_data *cleanup_data);

// data_utils.c
int			count_args(t_arg *args);
void		fill_args(t_ast *ast, char *argv[], int type);

// repl/utils/repl_get_pid.c
int			get_pid(void);
int			ft_pid(int set);

// utils
bool		ft_free(void **ptr);

char		*get_last_exit_str(void);
int			get_last_exit(void);
bool		full_exit_status(bool set_full_exit);

# ifndef SET_SUB_SHELL
#  define SET_SUB_SHELL 1
# endif

# ifndef UNSET_SUB_SHELL
#  define UNSET_SUB_SHELL 2
# endif

# ifndef GET_SUB_SHELL_MODE
#  define GET_SUB_SHELL_MODE 3
# endif

bool		sub_shell_mode(int flag);

//repl/repl_sub_shell.c
void		create_sub_shell(t_ast *ast);

void		print_error(bool shell_name,
				char *command_name, char *arg, char *str);

//repl/expansion/utils3.c
void		set_last_exit(int exit_status);

//parser
typedef struct s_parser	t_parser;
typedef struct s_ast	t_ast;

t_ast		*parser(char *str);

# ifndef GET_FDS
#  define GET_FDS 1
# endif

# ifndef SET_NEW_FDS
#  define SET_NEW_FDS 2
# endif

# ifndef REDIR_FDS
#  define REDIR_FDS 4
# endif

# ifndef CLEANUP_FDS
#  define CLEANUP_FDS 5
# endif

// repl/utils/pipe_utils.c
t_result	pipe_error_handler(t_pipe_data *vars);

// utils/fd1.c
t_fd_set	*io_data(int flag, void *data);
t_result	redir_fds(void);
t_result	reset_fds(void);
t_fd_set	*get_fds(void);
t_result	cleanup_fds(void);

t_result	ft_pipe(t_ast *ast);

char		*get_file_name(int fd);
void		print_fds(void);

/* ------------------------------ BUILT-INS ------------------------------ */
bool		ft_builtin_control(t_ast *ast);
int			ft_pwd(t_ast *ast);
int			ft_env(t_ast *ast);
void		ft_export(t_ast *ast, t_arg *cur_arg);
void		ft_unset(t_ast *ast);
void		ft_exit(t_ast *ast);
t_result	ft_echo(t_ast *ast, t_arg *cur_arg);
t_result	ft_cap_echo(t_ast *ast, t_arg *cur_arg);
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

/* ---------------------------- ENV FUNCTIONS ---------------------------- */

char		**ft_initialize_env(char **base_env);
char		*get_env_value(char **env, char *var_name,
				char *buffer, int buf_size);
char		**new_env_list_after_add(char *str_to_add, char **env, bool plus);
char		*get_env_var_name(char *line);
char		**new_env_list_after_delete(char *var_to_rm, char **env_before);
void		ft_update_env(char *var_name, char *new_value, char **env);
char		***get_env_list(char ***set_new_env);//added

/* ---------------------------- SIGNALS ----------------------------------- */

t_result	set_ctrl_c(void);
//t_result	set_ctrl_slash(void);
t_result	child_heredoc_signal_init(void);
void		init_terminal_settings(void);
void		reset_terminal_settings(void);
void		set_signals(void);
void		reset_signals(void);
bool		redisplay_prompt(bool set_state, bool new_state);

// ----------- additional utils -----------------
void		print_error_weird_quotes(bool shell_name, char *command_name,
				char *arg, char *str);
t_result	ft_cur_exit(t_ast *ast, int exit_value);

//FROM INTERNALS_H:
t_result	dollar_lexing(t_lexer *lexer, t_token *token);
void		basic_sign_type(t_lexer *lexer, t_token *token);
t_result	literal_type(t_lexer *lexer, t_token *token);
t_result	interpreted_type(t_lexer *lexer, t_token *token);
t_result	redir_type(t_lexer *lexer, t_token *token, bool recursive_call);
bool		is_redir_terminator_char(char c);
char		*get_potential_fd(t_lexer *lexer);
void		skip_leading_void_whitespace(t_lexer *lexer);
t_lexer		new_lexer(char *str);
t_result	subshell_type(t_lexer *lexer, t_token *token);
t_result	ident_wildcard_literals(t_lexer *lexer,
				t_token *token, bool skip_next_term);
int			name_len(char *str);
void		lexer_error(t_token *token);
char		*check_limis_potential_fd(char *left_redir_arg,
				t_lexer *lexer, t_lexer lexer_backup);

#endif
