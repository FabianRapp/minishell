/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 01:05:26 by frapp             #+#    #+#             */
/*   Updated: 2024/03/25 09:56:39 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

// updates the cur_path in the path_ob, if its NULL all paths have been checked
// frees the old path
bool	next_path(t_path *path_ob)
{
	ft_free((void **)&(path_ob->cur_path));
	if (!(path_ob->all_paths) || !(path_ob->all_paths)[path_ob->read_postion])
	{
		print_error(SHELL_NAME, path_ob->command_name, NULL,
			"No such file or directory");
		return (ft_cur_exit(path_ob->ast, 127), false);
	}
	path_ob->position = path_ob->read_postion;
	while ((path_ob->all_paths)[path_ob->read_postion] != ':'
		&& (path_ob->all_paths)[path_ob->read_postion])
	{
		path_ob->read_postion++;
	}
	path_ob->cur_path = ft_strndup(path_ob->all_paths + path_ob->position,
			path_ob->read_postion - path_ob->position);
	if (!path_ob->cur_path || !ft_strjoin_inplace(&(path_ob->cur_path), "/"))
		return (set_errno_as_exit(path_ob->ast, false), false);
	while ((path_ob->all_paths)[path_ob->read_postion] == ':')
		path_ob->read_postion++;
	return (true);
}

static bool	init_edgecases(t_ast *ast, char *command_name)
{
	char	*tmp;

	if (!ft_strcmp(command_name, ".."))
	{
		print_error(true, "..", NULL, "command not found");
		return (ft_cur_exit(ast, 127), false);
	}
	if (!ft_strcmp(command_name, "/") || !ft_strcmp(command_name, "~"))
	{
		ft_cur_exit(ast, 126);
		if (*command_name == '/')
			return (print_error(true, "/", NULL, "Is a directory"), false);
		tmp = get_env_value(*(ast->shared_data->envs), "HOME", 0, 0);
		print_error(true, tmp, NULL, "Is a directory");
		return (free(tmp), false);
	}
	return (true);
}

static char	*init_path(t_ast *ast, char *command_name, t_path *path_ob,
	char *path_var)
{
	if (init_edgecases(ast, command_name) == false)
		return (NULL);
	if (command_name && (*command_name == '/' || (*command_name == '.'
				&& ft_strlen(command_name) != 1)))
		return (handle_absolute_path(command_name));
	if (command_name && (*command_name == '/' || (*command_name == '.'
				&& ft_strlen(command_name) == 1)))
	{
		ft_fprintf(2, "%s: %c: filename argument required\n", SHELL_NAME,
			*command_name);
		ft_fprintf(2, "%c: usage: %c filename [arguments]\n", *command_name,
			*command_name);
		return (ft_cur_exit(ast, 2), NULL);
	}
	if (!init_path_object(ast, command_name, path_ob, path_var))
		return (free(path_ob->all_paths), free(path_ob->cur_path), NULL);
	if (!path_ob->cur_path || !*(path_ob->cur_path))
		return (free(path_ob->all_paths), free(path_ob->cur_path), NULL);
	return (command_name);
}

static void	check_if_default_exit_status(t_ast *ast, char *cmd_name)
{
	if (ast->exit_status == DEFAULT_EXIT_STATUS)
	{
		ft_cur_exit(ast, 127);
		print_error(SHELL_NAME, cmd_name, NULL, "No such file or directory");
	}
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
		path = ft_strjoin_free_s1(path_ob.cur_path, command_name);
		path_ob.cur_path = NULL;
		if (!path)
			return (set_errno_as_exit(ast, 0), free(path_ob.all_paths), NULL);
		if (!access(path, X_OK))
			return (free(path_ob.all_paths), path);
		if ((0 * ft_free((void **)&(path))) || (errno != ENOENT && errno != 20))
			return (set_errno_as_exit(ast, true), NULL);
		errno = 0;
		if (!next_path(&path_ob))
			return (free(path_ob.all_paths), free(path_ob.cur_path), NULL);
	}
	check_if_default_exit_status(ast, command_name);
	return (free(path_ob.all_paths), free(path_ob.cur_path), NULL);
}
