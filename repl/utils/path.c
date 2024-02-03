/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 01:05:26 by frapp             #+#    #+#             */
/*   Updated: 2024/02/02 08:18:31 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

// updates the cur_path in the path_ob, if its NULL all paths have been checked
// frees the old path
// returns EXIT_ERROR on malloc fail, otherwise NOT_FINISHED
bool	next_path(t_path *path_ob)
{
	if (!path_ob)
		return (false);
	my_free((void **)&(path_ob->cur_path));
	path_ob->cur_path = NULL;
	if (!(path_ob->all_paths) || !(path_ob->all_paths)[path_ob->read_postion])
	{
		path_ob->cur_path = NULL;
		return (true);
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
	{
		return (false);
	}
	ft_strjoin_inplace(&(path_ob->cur_path), "/");
	if (!path_ob->cur_path)
	{
		return (false);
	}
	while ((path_ob->all_paths)[path_ob->read_postion] == ':')
	{
		path_ob->read_postion++;
	}
	return (true);
}

bool	init_path(t_path *path_ob, char *env_var)
{
	path_ob->all_paths = getenv(env_var);
	path_ob->cur_path = NULL;
	path_ob->read_postion = 0;
	path_ob->position = 0;
	return (next_path(path_ob));
}

// changes the global errno
char	*find_path(t_ast *ast, char **command_name, char *path_env, t_child_data *data)
{
	t_path	path_ob;
	char	*command_path;

	*command_name = ast->name->token->str_data; // TODO: name neeeds to be fully expanded, currtly some cases are not expanded (for example exit status req., wildcards etc)
	if (!init_path(&path_ob, path_env))
	{
		data->exit_status = 1;
		return (NULL);
	}
	if (!path_ob.cur_path || !*(path_ob.cur_path))
		return (NULL);
	while (path_ob.cur_path && *(path_ob.cur_path))
	{
		command_path = ft_strjoin(path_ob.cur_path, *command_name);
		if (!command_path)
		{
			data->exit_status = 1;
			return (NULL);
		}
		my_free((void **)&(path_ob.cur_path));
		if (!access(command_path, X_OK))
			return (command_path);
		my_free((void **)&(command_path));
		if (errno != ENOENT)
		{
			// if the file exists but some error occurred
			// TODO: would be better to checck rest of the paths but idk what bash does
			print_error(true, "idk what error", "in find_path()", NULL);
			data->exit_status = 1;
			return (NULL);
		}
		if (!next_path(&path_ob))
		{
			data->exit_status = 1;
			return (NULL);
		}
	}
	return (NULL);
}
