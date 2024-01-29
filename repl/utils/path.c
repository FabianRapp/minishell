/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 01:05:26 by frapp             #+#    #+#             */
/*   Updated: 2024/01/29 07:16:41 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

// updates the cur_path in the path_ob, if its NULL all paths have been checked
// frees the old path
// returns EXIT_ERROR on malloc fail, otherwise NOT_FINISHED
void	next_path(t_path *path_ob, int *info)
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
	{
		(*info) = EXIT_ERROR;
		return ;
	}
	ft_strjoin_inplace(&(path_ob->cur_path), "/");
	if (!path_ob->cur_path)
	{
		(*info) = EXIT_ERROR;
		return ;
	}
	while ((path_ob->all_paths)[path_ob->read_postion] == ':')
	{
		path_ob->read_postion++;
	}
	(*info) = NOT_FINISHED;
	return ;
}

void	init_path(t_path *path_ob, int *info)
{
	path_ob->all_paths = getenv("PATH");
	path_ob->cur_path = NULL;
	path_ob->read_postion = 0;
	path_ob->position = 0;
	next_path(path_ob, info);
	return ;
}

// changes the global errno
char	*find_path(t_ast *ast, char **command_name, int *info)
{
	t_path	path_ob;
	char	*command_path;

	*command_name = ast->name->token->str_data; // TODO: name neeeds to be fully expanded, currtly some cases are not expanded (for example exit status req., wildcards etc)
	init_path(&path_ob, info);
	if ((*info) != NOT_FINISHED)
		return (NULL);
	if (!path_ob.cur_path || !*(path_ob.cur_path))
		return (NULL);
	while (path_ob.cur_path && *(path_ob.cur_path))
	{
		command_path = ft_strjoin(path_ob.cur_path, *command_name);
		if (!command_path)
		{
			(*info) = EXIT_ERROR;
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
			perror(strerror(errno));
			*info = SYNTAX_ERROR;
			return (NULL);
		}
		next_path(&path_ob, info);
		if (*info != NOT_FINISHED)
			return (NULL);
	}
	perror("command not found");
	return (NULL);
}
