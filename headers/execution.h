/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:24:35 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/27 00:33:50 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"

/******************************************************************************/
/* *********************             EXECUTION:            ****************** */
/******************************************************************************/

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

# ifndef SET_SUB_SHELL
#  define SET_SUB_SHELL 1
# endif

# ifndef UNSET_SUB_SHELL
#  define UNSET_SUB_SHELL 2
# endif

# ifndef GET_SUB_SHELL_MODE
#  define GET_SUB_SHELL_MODE 3
# endif

typedef enum e_type				t_type;
typedef struct s_arg			t_arg;
typedef struct s_ast			t_ast;
typedef enum e_result			t_result;
typedef struct s_fd_set			t_fd_set;
typedef struct s_cleanup_data	t_cleanup_data;

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
int			extend_fd_array(t_fd_set **fds);
t_fd_set	*add_fd_pair(t_fd_set *fds, t_fd_set new_fd_pair);
t_result	handle_open_error(t_ast *ast, t_fd_set *fd_pair);
t_result	handle_closed_fd_arg(t_fd_set *fds, t_fd_set *new_fd_pair);
void		*add_fd_pair_error(t_fd_set *fds, t_fd_set new_fd_pair);
t_result	check_valid_arg(t_ast *ast, t_redir *redir);

//Execution utils:
int			count_args(t_arg *args);
void		fill_args(t_ast *ast, char *argv[], int type);
char		*extract_command_name(char *path);
int			ft_pid(int set);
int			get_pid(void);
// t_ast	*handle_manunal_input(char **av, t_cleanup_data *cleanup_data);//?
t_ast		*get_input2(t_cleanup_data *cleanup_data, char *first_input);
t_ast		*get_input(t_cleanup_data *cleanup_data);
void		main_exit(t_cleanup_data *data, bool full_exit, bool ft_exit_call);
bool		file_in_pwd(char *file);
char		*handle_absolute_path(char *path);
t_result	init_path_object(t_ast *ast, char *command_name,
				t_path *path_ob, char *path_var);
bool		next_path(t_path *path_ob);
char		*find_path(t_ast *ast, char *command_name, char *path_var);
t_result	pipe_error_handler(t_pipe_data *vars);
t_result	check_brackets(t_ast *ast, char *input);

#endif	//EXECUTION_H
