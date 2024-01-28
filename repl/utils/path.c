/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 01:05:26 by frapp             #+#    #+#             */
/*   Updated: 2024/01/28 01:05:43 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

// updates the cur_path in the path_ob, if its NULL all paths have been checked
// frees the old path
void	next_path(t_path *path_ob)
{
	if (!path_ob)
		return ;
	my_free((void **)&(path_ob->cur_path));
	path_ob->cur_path = NULL;
	if (!(path_ob->all_paths) || !(path_ob->all_paths)[path_ob->read_postion])
	{
		path_ob->cur_path = NULL;
		return ;
	}
	path_ob->position = path_ob->read_postion;
	while ((path_ob->all_paths)[path_ob->read_postion] != ':'
		&& (path_ob->all_paths)[path_ob->read_postion])
	{
		path_ob->read_postion++;
	}
	path_ob->cur_path = ft_strndup(path_ob->all_paths + path_ob->position,
		path_ob->read_postion - path_ob->position);
	if (!path_ob->cur_path)
		return ; //cleanup
	ft_strjoin_inplace(&(path_ob->cur_path), "/");
	if (!path_ob->cur_path)
		return ; //cleanup
	while ((path_ob->all_paths)[path_ob->read_postion] == ':')
	{
		path_ob->read_postion++;
	}
}

void	init_path(t_path *path_ob)
{
	path_ob->all_paths = getenv("PATH");
	path_ob->cur_path = NULL;
	path_ob->read_postion = 0;
	path_ob->position = 0;
	next_path(path_ob);
}

// changes the global errno
char	*find_path(char *command_name)
{
	t_path	path_ob;
	char	*command_path;

	init_path(&path_ob);
	if (!path_ob.cur_path || !*(path_ob.cur_path))
		return (NULL);
	while (path_ob.cur_path && *(path_ob.cur_path))
	{
		command_path = ft_strjoin(path_ob.cur_path, command_name);
		if (!command_path)
			return (cleanup(), NULL);
		if (!access(command_path, X_OK))
			return (command_path);
		my_free((void **)&(command_path));
		next_path(&path_ob);
	}
	return (NULL);
}