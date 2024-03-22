/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:26:07 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/22 22:43:56 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

char	*get_parent_dir_path(void)
{
	char	*cwd;
	char	*parent;
	char	*cut;

	cwd = getcwd(NULL, PATH_MAX);
	cut = ft_strrchr(cwd, 47);
	parent = ft_substr(cwd, 0, cut - cwd);
	free(cwd);
	return (parent);
}

int	ft_update_dir_vars(t_ast *ast, char *before, char *after)
{
	ft_update_env("OLDPWD", before, *(ast->shared_data->envs));
	ft_update_env("PWD", after, *(ast->shared_data->envs));
	ft_update_env("OLDPWD", before, *(ast->shared_data->env_exp));
	ft_update_env("PWD", after, *(ast->shared_data->env_exp));
	free(after);
	free(before);
	return (0);
}

char	*init_ft_cd_step(t_ast *ast, char *step, int index)
{
	char	*to_go;

	to_go = ft_strdup(step);
	if (index == 0 && !ft_strcmp(to_go, "~"))
	{
		free(to_go);
		to_go = get_env_value(*(ast->shared_data->envs), "HOME");
	}
	else if (!ft_strcmp(to_go, "."))
	{
		free(to_go);
		to_go = get_env_value(*(ast->shared_data->envs), "PWD");
	}
	else if (!ft_strcmp(to_go, ".."))
	{
		free(to_go);
		to_go = get_parent_dir_path();
	}
	if (!to_go)
		return (set_errno_as_exit(ast, false), NULL);
	return (to_go);
}
