/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 01:05:26 by frapp             #+#    #+#             */
/*   Updated: 2024/02/15 06:00:13 by frapp            ###   ########.fr       */
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
		path_ob->ast->exit_status = 127;
		return (set_last_exit(path_ob->ast->exit_status),
			print_error(SHELL_NAME, path_ob->command_name, NULL, "command not found"), false);
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
		path_ob->ast->exit_status = errno;
		return (print_error(true, NULL, NULL, strerror(path_ob->ast->exit_status)), false);
	}
	ft_strjoin_inplace(&(path_ob->cur_path), "/");
	if (!path_ob->cur_path)
	{
		path_ob->ast->exit_status = errno;
		return (print_error(true, NULL, NULL, strerror(path_ob->ast->exit_status)), false);
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
char	*find_path(t_ast *ast, char *command_name, char *path_env)
{
	t_path	path_ob;
	char	*command_path;

	path_ob.ast = ast;
	path_ob.command_name = command_name;
	if (!init_path(&path_ob, path_env))
	{
		return (NULL);
	}
	if (!path_ob.cur_path || !*(path_ob.cur_path))
		return (NULL);
	while (path_ob.cur_path && *(path_ob.cur_path))
	{
		command_path = ft_strjoin(path_ob.cur_path, command_name);
		if (!command_path)
		{
			ast->exit_status = errno;
			return (NULL);
		}
		my_free((void **)&(path_ob.cur_path));
		if (!access(command_path, X_OK))
		{
			errno = 0;
			return (command_path);
		}
		my_free((void **)&(command_path));
		if (errno != ENOENT && errno != 20)
		{
			ast->exit_status = errno;
			return (print_error(true, "DEBUG find_path", NULL, strerror(errno)), NULL);
		}
		errno = 0;
		if (!next_path(&path_ob))
			return (NULL);
		errno = 0;
	}
	return (NULL);
}
