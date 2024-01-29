/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 19:38:29 by frapp             #+#    #+#             */
/*   Updated: 2024/01/29 07:17:25 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVAL_H
# define EVAL_H

# include "minishell.h"

typedef struct s_cleanup_data	t_cleanup_data;

typedef struct s_path
{
	char	*all_paths;
	char	*cur_path;
	int		position;
	int		read_postion;
}	t_path;

void	*expand_strs(t_ast *ast);
char	*find_path(t_ast *ast, char **command_name, int *info);
void	ft_buildin(t_ast *ast);
int		count_args(t_ast *ast, int type);

// utils

// input_exit.c
t_ast	*get_input(t_cleanup_data *cleanup_data);
void	main_cleanup(t_cleanup_data *data);

// data_utils.c
int		count_args(t_ast *ast, int type);
void	fill_args(t_ast *ast, char *argv[], int type);


#endif