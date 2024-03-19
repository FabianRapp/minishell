/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 01:05:26 by frapp             #+#    #+#             */
/*   Updated: 2024/03/19 01:24:27 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

char	*handle_absolute_path(char *path)
{
	if (!access(path, X_OK))
		return (ft_strdup(path));
	ft_fprintf(2, "%s: %s\n", SHELL_NAME, strerror(errno));
	set_last_exit(127);
	errno = 0;
	return (NULL);
}

// updates the cur_path in the path_ob, if its NULL all paths have been checked
// frees the old path
// returns EXIT_ERROR on malloc fail, otherwise NOT_FINISHED
bool	next_path(t_path *path_ob)
{
	ft_free((void **)&(path_ob->cur_path));
	if (!(path_ob->all_paths) || !(path_ob->all_paths)[path_ob->read_postion])
	{
		path_ob->ast->exit_status = 127;
		return (set_last_exit(127), print_error(SHELL_NAME,
				path_ob->command_name, NULL, "No such file or directory"), false);
	}
	path_ob->position = path_ob->read_postion;
	while ((path_ob->all_paths)[path_ob->read_postion] != ':'
		&& (path_ob->all_paths)[path_ob->read_postion])
		path_ob->read_postion++;
	path_ob->cur_path = ft_strndup(path_ob->all_paths + path_ob->position,
			path_ob->read_postion - path_ob->position);
	if (!path_ob->cur_path || !ft_strjoin_inplace(&(path_ob->cur_path), "/"))
	{
		path_ob->ast->exit_status = errno;
		errno = 0;
		return (false);
	}
	while ((path_ob->all_paths)[path_ob->read_postion] == ':')
		path_ob->read_postion++;
	return (true);
}

t_result	init_path_object(t_ast *ast, char *command_name, t_path *path_ob,
	char *path_var)
{
	path_ob->ast = ast;
	path_ob->command_name = command_name;
	path_ob->cur_path = NULL;
	path_ob->all_paths = NULL;
	path_ob->read_postion = 0;
	path_ob->position = 0;
	if (!command_name)
		return (ERROR);
	if (!*command_name)
	{
		print_error(true, "", NULL, "command not found");
		ast->exit_status = 127;
		set_last_exit(127);
		return (ERROR);
	}
	path_ob->all_paths = get_env_value(NULL, path_var);
	if (!path_ob->all_paths && errno)
	{
		path_ob->ast->exit_status = errno;
		errno = 0;
		return (ERROR);
	}
	return (next_path(path_ob));
}

char	*init_path(t_ast *ast, char *command_name, t_path *path_ob,
	char *path_var)
{
	if (command_name && (*command_name == '/' || *command_name == '.'))
		return (handle_absolute_path(command_name));
	if (!init_path_object(ast, command_name, path_ob, path_var))
		return (free(path_ob->all_paths), free(path_ob->cur_path), NULL);
	if (!path_ob->cur_path || !*(path_ob->cur_path))
		return (free(path_ob->all_paths), free(path_ob->cur_path), NULL);
	return (command_name);
}

char	*find_path(t_ast *ast, char *command_name, char *path_var)
{
	t_path	path_ob;
	char	*path;
	char	*temp;

	temp = init_path(ast, command_name, &path_ob, path_var);
	if (!temp || temp != command_name)
		return (temp);
	while (path_ob.cur_path && *(path_ob.cur_path))
	{
		path = ft_strjoin(path_ob.cur_path, command_name);
		if (!path)
		{
			ast->exit_status = errno;
			errno = 0;
			return (free(path_ob.all_paths), free(path_ob.cur_path), NULL);
		}
		if (0 * ft_free((void **)&(path_ob.cur_path)) || !access(path, X_OK))
			return (free(path_ob.all_paths), path);
		if ((0 * ft_free((void **)&(path))) || (errno != ENOENT && errno != 20))
			return (set_errno_as_exit(ast, true), NULL);
		errno = 0;
		if (!next_path(&path_ob))
			return (free(path_ob.all_paths), free(path_ob.cur_path), NULL);
	}
	return (free(path_ob.all_paths), free(path_ob.cur_path), NULL);
}
