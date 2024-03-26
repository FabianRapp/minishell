/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:24:35 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/26 18:59:04 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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

/* *************************    PROJECTS'S HEADER'S    ********************* */
# include "libft.h"
// # include "utils.h"
// # include "parser.h"

/* *************************    MACROS' DEFINITIONS    ********************* */
# ifndef TESTER
#  define TESTER 1
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
typedef enum e_type		t_type;
typedef	struct s_token_list	t_token_list;
typedef struct s_arg	t_arg;
typedef struct s_redir t_redir;
typedef struct s_ast	t_ast;

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
char		*get_env_var_name(char *line);
char		**new_env_list_after_delete(char *var_to_rm, char **env_before);
void		ft_update_env(char *var_name, char *new_value, char **env);
char		***get_env_list(char ***set_new_env);

/******************************************************************************/
/* *********************              LEXER:               ****************** */
/******************************************************************************/
typedef enum e_type
{
	UNKNOWN = false,
	T_EOF,
	WHITE_SPACE,
	PIPE,
	OR,
	AND,
	ENV_VAR,
	EXIT_STATUS_REQUEST,
	PID_REQUEST,
	WILDCARD,
	VOID,
	INTERPRETED,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_IN_OUT,
	HERE_DOC,
	SUBSHELL,
	WORD,
	COMMAND,
	ARGUMENT,
	REDIR_ARG,
	DUMMY_COMMAND,
	LITERAL,
	SEMICOL,
	HERE_STR,
}	t_type;

// typedef struct s_token	t_token;
typedef struct s_token
{
	t_type		type;
	char		*str_data;
	char		unknown;
	char		*input_str;
	int			input_position;
	char		*left_redir_arg;
	bool		here_doc_arg_literal;
	char		*old_data;
}	t_token;

typedef struct s_lexer
{
	char	*str;
	int		position;
	int		read_position;
	char	cur_char;
	char	last_char;
}	t_lexer;

t_result	dollar_lexing(t_lexer *lexer, t_token *token);
char		*get_potential_fd(t_lexer *lexer);
char		*check_limis_potential_fd(char *left_redir_arg,
	t_lexer *lexer, t_lexer lexer_backup);
void		print_error_redir_arg(t_lexer *lexer);
t_result	redir_type(t_lexer *lexer, t_token *token, bool recursive_call);
void		basic_sign_type(t_lexer *lexer, t_token *token);
t_result	literal_type(t_lexer *lexer, t_token *token);
t_result	interpreted_type(t_lexer *lexer, t_token *token);
t_result	ident_wildcard_literals(t_lexer *lexer,
	t_token *token, bool skip_next_term);
t_token		*next_new_token(t_lexer *lexer, bool recursive_call);
void		read_char(t_lexer *lexer);
bool		is_redir_terminator_char(char c);
void		lexer_error(t_token *token);
t_lexer		new_lexer(char *str);

/******************************************************************************/
/* *********************              PARSER:              ****************** */
/******************************************************************************/
typedef struct s_arg
{
	t_type			type;
	t_token_list	*name;
	struct s_arg	*next;
}	t_arg;

typedef struct s_parser	t_parser;
typedef struct s_parser
{
	t_token		*token;
	t_type		p_type;
	t_parser	*next;
	t_parser	*arg;
	t_parser	*rest_name;
}	t_parser;

typedef	struct s_token_list
{
	int					arr[1000];
	struct s_token_list	*next;
	t_token				*token;
}	t_token_list;

typedef struct s_left_right_parsers
{
	t_parser	*left;
	t_parser	*right;
}	t_left_right_parsers;

//from directory list_operations:
t_parser			*init_parser(char *str);
t_result			insert_dummy_here(t_parser *parser);
void				move_next_to_restname(t_parser *parser, t_parser **rest_name);
void				move_commands_infront(t_parser *parser);
void				move_to_arg(t_parser *parser, bool is_terminator(t_type),
	t_type new_type, bool as_must_as_possible);
void				remove_whitespace(t_parser *parser);
void				free_parser_main(t_parser *parser, bool free_tokens);
void				free_ncircular_parser(t_parser *parser, bool free_tok);
void				trim_whitespace(t_parser *parser);
t_parser			*last_parser(t_parser *parser);
t_parser			*insert_token(t_parser **parser, t_token *token);
void				remove_parser_node(t_parser **node, bool free_tok);
void				jump_to_start(t_parser **parser);

// From directory parser_ast:
t_result			parser_resovle_here_doc(t_redir *redir);
t_result			append_redir(t_ast *ast_node, t_parser *args, t_redir **cur_redir);
t_result			parser_resovle_here_str(t_redir *redir);
t_token_list		*extract_token_list(t_parser *parser, char name_or_arg);
t_left_right_parsers	split_parser(t_parser *split_location);
t_parser			*find_highest_operator(t_parser *parser);
t_arg				*append_arg(t_parser *parser, t_arg *head_arg);
char				*parser_expand_line(char *line);
t_ast				*build_ast(t_parser *parser);

// REST IN PARSER DIRECTORY:
t_ast				*parser(char *str);
t_result			type_commands(t_parser *parser);
t_result			parse_redir_paths(t_parser *parser);
void				type_args(t_parser *parser);
t_result			has_content(t_parser *parser);
// bool				has_redir_arg(t_parser *parser);
t_parser			*has_none_redir_arg(t_parser *parser);
void				type_command(t_parser *parser, bool *found_command);






/******************************************************************************/
/* *********************            EXPANSION:             ****************** */
/******************************************************************************/
# ifndef RETURN_NOW
#  define RETURN_NOW 1
# endif

# ifndef CONTINUE
#  define CONTINUE 2
# endif

# ifndef GO_ON
#  define GO_ON 3
# endif

t_result	expansion(t_ast *ast);
t_result	expand_args(t_ast *ast, t_arg **base_arg, bool here_doc);
t_result	word_splitting(t_token_list **list);
void		add_arg_front(t_arg **head, t_arg *new_arg);
t_result	env_to_word_token(t_token *token);
t_result	pidreq_to_literal_token(t_shared_data *env, t_token *token);
int	check_empty_arg(t_arg *last, t_arg **cur,
		t_ast *ast, t_arg **base_arg);
t_result	merge_literals(t_token_list *node);
t_result	expand_interpreted(t_token *token);
t_token_list	*move_nodes_ahead(t_token_list *list, bool set_this_true);
t_token_list	*remove_non_literals(t_token_list *list);
t_result	expansion_move_to_arg(t_arg **next_arg, t_token_list *list);
// void	expandlist_convert_white_space(t_token *token); //? it doesn't seem to bee used anywhere
void	init_expand_args(t_arg **cur, t_arg **base_arg, t_arg **last);

// WILDCARDS:
typedef struct s_wildcard_parameters
{
	char	*prefix;
	char	*suffix;
	char	**sub_str;
}	t_wildcard_parameters;

typedef struct s_wildcard_node_expansion
{
	t_token_list			*w_head;
	t_token_list			*base_next;
	t_token_list			*cur;
	DIR						*cur_dir;
	char					*w_str;
	t_wildcard_parameters	w_para;
}	t_wildcard_node_expasion;

bool			matches_wildcard(char *str, t_wildcard_parameters *w_para);
t_result		fill_wildcard_data(char *wldcd_str,
	t_wildcard_parameters *w_para);
// void			print_wildcard_data(t_wildcard_parameters *data);//? it doesn't seem to be used somewhere
t_result		wildcards(t_token_list *name);
char			*next_file_name(DIR *dir, bool hidden);
void			clean_wildcard_data(t_wildcard_parameters *w_para);
t_result		trim_identifiers(t_token_list *name, bool set_this_true);



/******************************************************************************/
/* *********************             EXECUTION:            ****************** */
/******************************************************************************/
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

typedef struct s_path
{
	char	*all_paths;
	char	*cur_path;
	int		position;
	int		read_postion;
	char	*command_name;
	t_ast	*ast;
	char	path_buffer[PATH_MAX + 1];
}	t_path;

typedef struct s_pipe_data
{
	int			pipe_fd[2];
	int			base_write;
	int			base_read;
	int			left_pid;
	t_ast		*ast;
}	t_pipe_data;

void		create_sub_shell(t_ast *ast);
t_result	ft_pipe(t_ast *ast);
void		run_node(t_ast *ast);
int			run_command_node(t_ast *ast);

// Redirections folder:
t_result	resolve_redirs(t_ast *ast);
t_fd_set	redir_fd_write(char *file, bool append, int base_fd);
t_fd_set	redir_read(char *file, int base_fd, bool in_out);
int	extend_fd_array(t_fd_set **fds);
t_fd_set	*add_fd_pair(t_fd_set *fds, t_fd_set new_fd_pair);
t_result	handle_open_error(t_ast *ast, t_fd_set *fd_pair);
t_result	handle_closed_fd_arg(t_fd_set *fds, t_fd_set *new_fd_pair);
void	*add_fd_pair_error(t_fd_set *fds, t_fd_set new_fd_pair);
t_result	check_valid_arg(t_ast *ast, t_redir *redir);

//Execution utils:
int	count_args(t_arg *args);
void	fill_args(t_ast *ast, char *argv[], int type);
char	*extract_command_name(char *path);
int	ft_pid(int set);
int	get_pid(void);
// t_ast	*handle_manunal_input(char **av, t_cleanup_data *cleanup_data); //! it doesn't seem to be used somewhere
t_ast	*get_input(t_cleanup_data *cleanup_data);
void	main_exit(t_cleanup_data *data, bool full_exit, bool ft_exit_call);
bool	file_in_pwd(char *file);
char	*handle_absolute_path(char *path);
t_result	init_path_object(t_ast *ast, char *command_name,
	t_path *path_ob, char *path_var);
bool	next_path(t_path *path_ob);
char	*find_path(t_ast *ast, char *command_name, char *path_var);
t_result	pipe_error_handler(t_pipe_data *vars);
t_result	check_brackets(t_ast *ast, char *input);


/******************************************************************************/
/* *********************              SIGNALS:             ****************** */
/******************************************************************************/
void		set_signals(void);
t_result	set_ctrl_c(void);
void		signal_handler_ctrl_c(int signal, siginfo_t *info, void *data);
t_result	set_ctrl_slash(void);
bool		redisplay_prompt(bool set_state, bool new_state);
void		init_terminal_settings(void);
void		reset_terminal_settings(void);
void		reset_signals(void);

/******************************************************************************/
/* *********************               UTILS:              ****************** */
/******************************************************************************/




//BEFORE:
void					free_ast(t_ast *ast);

#endif
