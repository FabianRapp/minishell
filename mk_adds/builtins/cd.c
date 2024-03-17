/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:26:07 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/17 19:58:43 by frapp            ###   ########.fr       */
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


//TODO i need to handle also cases like ../../ or ../something or ~/../
t_result	ft_cd(t_ast *ast)
{
	char	*to_go;
	char	*before;
	char	*after;

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
	ft_update_env_var("OLDPWD", before, *(ast->shared_data->envs));
	ft_update_env_var("OLDPWD", before, *(ast->shared_data->env_exp));
	ft_update_env_var("PWD", after, *(ast->shared_data->envs));
	ft_update_env_var("PWD", after, *(ast->shared_data->env_exp));
	free(before);
	free(after);
	ast->exit_status = 0;
	set_last_exit(0);
	return (SUCCESS);
}

//!		if i type: 
//!		cd somethingrandom
//!		i get my message: minishell: cd: oldgn: No such file or directory
//!		but for some reason i also get these: 
//!		minishell: No such file or directory
//!		rest_fds error
//!		
//!
//!
//!

