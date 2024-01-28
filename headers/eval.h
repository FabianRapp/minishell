/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 19:38:29 by frapp             #+#    #+#             */
/*   Updated: 2024/01/28 04:08:44 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVAL_H
# define EVAL_H

# include "minishell.h"

typedef struct s_path
{
	char	*all_paths;
	char	*cur_path;
	int		position;
	int		read_postion;
}	t_path;

void	*expand_strs(t_ast *ast);
char	*find_path(char *command_name);
int		ft_buildin(t_ast *ast);
int		count_args(t_ast *ast, int type);

#endif