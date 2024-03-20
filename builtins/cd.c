/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:26:07 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/20 12:28:58 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
cd = change directory [either realtive or absolute path]

examples:
----------
- cd (alone) or cd ~ or cd -- : 	change to the home fdirectory
- cd .. : 							change to the parent/above directory
- cd - : 							change to the last/previous working directory
									AND prints the new directory. (cd and pwd directly)
						
cd with 1 or 3 or more slashes go to /    pwd: /
cd // -->pwd: //
*/

#include "../headers/minishell.h"
#include "../headers/eval.h"

static char	*get_parent_dir_path(void)
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

static void	ft_update_dir_vars(t_ast *ast, char *before, char *after)
{
	//First I add the two variables if they don't exist already:
	// *(ast->shared_data->envs) = new_env_list_after_add("OLDPWD",
	// 		*(ast->shared_data->envs));
	// *(ast->shared_data->envs) = new_env_list_after_add("PWD",
	// 		*(ast->shared_data->envs));
	// *(ast->shared_data->env_exp) = new_env_list_after_add("OLDPWD",
	// 		*(ast->shared_data->env_exp));
	// *(ast->shared_data->env_exp) = new_env_list_after_add("PWD",
	// 		*(ast->shared_data->env_exp));
	// ? i commented the above 8 lines, because with them 1 test is failing, and without them it works. 
	//So then, I can call this function to update them:
	ft_update_env_var("OLDPWD", before, *(ast->shared_data->envs));
	ft_update_env_var("PWD", after, *(ast->shared_data->envs));
	ft_update_env_var("OLDPWD", before, *(ast->shared_data->env_exp));
	ft_update_env_var("PWD", after, *(ast->shared_data->env_exp));
}

static void	ft_cd_to_var(t_ast *ast, bool not_alone, char *path, char *var)
{
	char	*to_go;
	char	*before;
	char	*after;

	before = getcwd(NULL, PATH_MAX);
	//? maybe i should/could check here whether HOME is unset before i call its value?
	if (ft_strcmp(var, "HOME") && ft_strcmp(var, "PWD") && ft_strcmp(var, "OLDPWD"))
		to_go = var;
	else
		to_go = get_env_value(*(ast->shared_data->envs), var);
	if (chdir(to_go) < 0)
	{
		ft_cur_exit(ast, 1);
		if (not_alone)
			print_error(true, "cd", path, "No such file or directory");
		else //this for the case that it could not go. e.g. unset HOME
			print_error(true, "cd", NULL, strerror(errno));
		free(before);
		free(to_go);
		return ;
	}
	after = getcwd(NULL, PATH_MAX);
	ft_update_dir_vars(ast, before, after);
	if (ft_strcmp(var, "OLDPWD") == 0) //for the case of: cd -
		ft_pwd(ast);
	free(before);
	free(to_go);
	free(after);
}


static int	ft_cd_step(t_ast *ast, char *step, int index, char *cd_arg)
{
	char	*to_go;
	char	*before;
	char	*after;

	to_go = ft_strdup(step);
	if (index == 0 && !ft_strcmp(to_go, "~"))
		to_go = get_env_value(*(ast->shared_data->envs), "HOME");
	else if (!ft_strcmp(to_go, "."))
		to_go = get_env_value(*(ast->shared_data->envs), "PWD");
	else if (!ft_strcmp(to_go, ".."))
		to_go = get_parent_dir_path();
	before = getcwd(NULL, PATH_MAX);
	if (chdir(to_go) < 0)
		return (ft_cur_exit(ast, 1), free(to_go), free(before), 
			print_error(true, "cd", cd_arg, "No such file or directory"), 0);
	after = getcwd(NULL, PATH_MAX);
	ft_update_dir_vars(ast, before, after);
	free(before);
	free(after);
	return (1);
}

// cd in bash cares only about the first argument. if there are more it ignores them.
int	ft_cd(t_ast *ast)
{
	t_arg	*args;
	char	**steps;
	char	*cd_arg;
	int		i;

	ft_cur_exit(ast, 0);
	args = ast->arg;
	if (!args) //if there is no argument cd changes to HOME
		return (ft_cd_to_var(ast, false, NULL, "HOME"), 0);
	cd_arg = ast->arg->name->token->str_data;
	if (*cd_arg == '\0') //cd doesn't do anything with empty argument
		return 0;
	if (ft_strcmp(cd_arg, "-") == 0)
		return (ft_cd_to_var(ast, false, NULL, "OLDPWD"), 0);
	if (ft_strcmp(cd_arg, "--") == 0)
		return (ft_cd_to_var(ast, false, NULL, "HOME"), 0);
	if (*cd_arg == '/')
		ft_cd_step(ast, "/", 0, cd_arg);
	steps = ft_split(cd_arg, '/');
	if (!steps)
		return (0);
	i = -1;
	while (steps[++i])
		ft_cd_step(ast, steps[i], i, cd_arg);
	ft_free_2darr(steps);
	return (0);
}


//!		if i unset PATH, and then do:
//!		cd -
//!		i get the message: 
//!		bash: cd: OLDPWD not set
//!		
//!		also, if I unset HOME and i do:
//!		cd $HOME
//!		i take the message:
//!		bash: cd: HOME not set
