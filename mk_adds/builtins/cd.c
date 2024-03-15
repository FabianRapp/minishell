/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:26:07 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/15 05:53:01 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
cd = change directory [either realtive or absolute path]

examples:
----------
- cd (alone) or cd ~ : 	change to the home fdirectory
- cd .. : 				change to the parent/above directory
- cd - : 				change to the last/previous working directory
						AND prints the new directory. (cd and pwd directly)
*/

#include "../../headers/minishell.h"
#include "../../headers/eval.h"

static char	*get_parent_dir_path(t_ast *ast)
{
	char	*cwd;
	char	*parent;
	char	*cut;

	cwd = get_env_value(ast, "PWD");
	cut = ft_strrchr(cwd, 47); // 47 is for the normal /
	parent = ft_substr(cwd, 0, cut - cwd);
	free(cwd);
	return (parent);
}

static void	ft_update_env_var(char *var_name, char *new_value, char **env)
{
	int		i;
	char	*half;
	
	i = 0;
	while (env[i] && ft_strncmp(var_name, env[i], ft_strlen(var_name)))
		i++;
	free(env[i]);
	half = ft_strjoin(var_name, "=");
	env[i] = ft_strjoin(half, new_value);
	free(half);
}

t_result	ft_cd(t_ast *ast)
{
	char	*to_go;
	char	*before;
	char	*after;
	bool	print_new;

	print_new = false;
	to_go = ft_strdup(ast->arg->name->token->str_data);
	if (!to_go || !ft_strcmp(to_go, "~") || !ft_strcmp(to_go, "~/")) //if there are no arguments, meaning cd alone
		to_go = get_env_value(ast, "HOME");
	
	else if (!ft_strcmp(to_go, "-"))
	{
		to_go = get_env_value(ast, "OLDPWD");
		printf("%s\n", to_go);
	}
	else if (!ft_strcmp(to_go, ".") || !ft_strcmp(to_go, "./"))
		to_go = get_env_value(ast, "PWD");
	else if (!ft_strcmp(to_go, "..") || !ft_strcmp(to_go, "../"))
		to_go = get_parent_dir_path(ast);
	if (chdir(to_go) < 0)
		return (print_error(true, "cd", to_go, "No such file or directory"), free(to_go), ERROR);
	before = get_env_value(ast, "PWD");
	after = getcwd(NULL, PATH_MAX);
	ft_update_env_var("OLDPWD", before, *(ast->envs));
	ft_update_env_var("OLDPWD", before, *(ast->env_exp));
	ft_update_env_var("PWD", after, *(ast->envs));
	ft_update_env_var("PWD", after, *(ast->env_exp));


	free(before);
	free(after);
	ast->exit_status = 0;
	set_last_exit(0);
	return (SUCCESS);
}
