/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 19:38:29 by frapp             #+#    #+#             */
/*   Updated: 2024/01/26 19:48:04 by frapp            ###   ########.fr       */
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


#endif