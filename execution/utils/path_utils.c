/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 03:31:52 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/26 01:56:23 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

char	*handle_absolute_path(char *path)
{
	if (!access(path, X_OK))
	{
		return (ft_strdup(path));
	}
	ft_fprintf(2, "%s: %s\n", SHELL_NAME, strerror(errno));
	set_last_exit(127);
	if (errno == 20)
		set_last_exit(126);
	errno = 0;
	return (NULL);
}

char	*handle_shell_fn(char *name)
{
	const char	*all_fns[] = {
		"cat",	"chmod",	"cp",	"df",	"grep",
		"history",	"ls",	"mkdir",	"mv",	"ps",
		"rm",	"sudo",	"touch",	"sleep",	NULL
	};
	int			i;
	char		*pwd;
	char		*tmp;

	i = -1;
	while (all_fns[++i])
	{
		if (!ft_strcmp(all_fns[i], name))
			break ;
	}
	if (!all_fns[i])
		return (NULL);
	tmp = get_env_value(NULL, "PWD", 0, 0);
	if (errno)
		return (set_last_exit(errno), free(tmp), NULL);
	pwd = ft_strjoin(":", tmp);
	free(tmp);
	if (errno)
		set_last_exit(errno);
	return (pwd);
}

t_result	init_path_object(t_ast *ast, char *command_name,
	t_path *path_ob, char *path_var)
{
	char	*tmp;

	path_ob->ast = ast;
	path_ob->command_name = command_name;
	path_ob->cur_path = NULL;
	path_ob->all_paths = NULL;
	path_ob->read_postion = 0;
	path_ob->position = 0;
	if (!command_name)
		return (ERROR);
	if (!*command_name)
		return (print_error(true, "", NULL, "command not found"),
			ast->exit_status = 127, set_last_exit(127), ERROR);
	path_ob->all_paths = get_env_value(NULL, path_var, 0, 0);
	if (errno)
		return (set_errno_as_exit(ast, false), ERROR);
	tmp = handle_shell_fn(command_name);
	if (errno)
		return (ft_free((void **)&(path_ob->all_paths)),
			set_errno_as_exit(ast, false), ERROR);
	if (!ft_strjoin_inplace(&(path_ob->all_paths), tmp))
		return (set_errno_as_exit(ast, false), free(tmp), ERROR);
	return (free(tmp), next_path(path_ob));
}
