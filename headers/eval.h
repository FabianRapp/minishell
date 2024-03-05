/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 19:38:29 by frapp             #+#    #+#             */
/*   Updated: 2024/03/05 07:42:27 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVAL_H
# define EVAL_H

# include "minishell.h"

typedef enum e_result			t_result;
typedef struct s_cleanup_data	t_cleanup_data;
typedef struct s_child_data		t_child_data;
typedef struct s_env			t_env;
typedef	struct s_arg			t_arg;

typedef struct s_path
{
	char	*all_paths;
	char	*cur_path;
	int		position;
	int		read_postion;
	char	*command_name;
	
	t_ast	*ast;
}	t_path;


t_result	expansion(t_ast *ast);
char		*find_path(t_ast *ast, char *command_name, char *path_env);
bool		init_path(t_path *path_ob, char *env_var);
bool		ft_buildin(t_ast *ast);

// utils

// input_exit.c
t_ast		*get_input(t_cleanup_data *cleanup_data);
void		main_exit(t_cleanup_data *data, bool full_exit);

// data_utils.c
int			count_args(t_arg *args);
void		fill_args(t_ast *ast, char *argv[], int type);

#endif