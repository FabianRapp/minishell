/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:26:07 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/20 06:08:28 by mevangel         ###   ########.fr       */
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
						
cd with 1 or 3 or more slashes go to /    pwd: /
cd // -->pwd: //
*/

#include "../headers/minishell.h"
#include "../headers/eval.h"

// static char	*get_parent_dir_path(t_ast *ast)
// {
// 	char	*cwd;
// 	char	*parent;
// 	char	*cut;

// 	cwd = get_env_value(*(ast->shared_data->envs), "PWD");
// 	cut = ft_strrchr(cwd, 47); // 47 is for the normal /
// 	parent = ft_substr(cwd, 0, cut - cwd);
// 	free(cwd);
// 	return (parent);
// }

// //! now that i deleted entirely the OLDPWD in the initialization, i must not do update. 
// //! there is somewhere a 14 returned as exit_code. for example cd ..
// //! the first time minishell or bash opens there is no OLDPWD. (only in the export list, without a value)
// //! cd no arguments segfaults currently
// //TODO i need to handle also cases like ../../ or ../something or ~/../


// void	ft_update_env_var(char *var_name, char *new_value, char **env)
// {
// 	int		i;
// 	char	*half;
	
// 	i = 0;
// 	while (env[i] && ft_strncmp(var_name, env[i], ft_strlen(var_name)))
// 		i++;
// 	free(env[i]);
// 	half = ft_strjoin(var_name, "=");
// 	env[i] = ft_strjoin(half, new_value);
// 	free(half);
// }



// t_result	ft_cd(t_ast *ast)
// {
// 	char	*to_go;
// 	char	*before;
// 	char	*after;

// 	to_go = ft_strdup(ast->arg->name->token->str_data);
// 	if (!to_go || !ft_strcmp(to_go, "~") || !ft_strcmp(to_go, "~/")) //if there are no arguments, meaning cd alone
// 		to_go = get_env_value(*(ast->shared_data->envs), "HOME");
// 	else if (!ft_strcmp(to_go, "-"))
// 	{
// 		to_go = get_env_value(*(ast->shared_data->envs), "OLDPWD");
// 		printf("%s\n", to_go);
// 	}
// 	else if (!ft_strcmp(to_go, ".") || !ft_strcmp(to_go, "./"))
// 		to_go = get_env_value(*(ast->shared_data->envs), "PWD");
// 	else if (!ft_strcmp(to_go, "..") || !ft_strcmp(to_go, "../"))
// 		to_go = get_parent_dir_path(ast);
// 	if (chdir(to_go) < 0)
// 	{
// 		ast->exit_status = 1;
// 		set_last_exit(1);
// 		return (print_error(true, "cd", to_go, "No such file or directory"), free(to_go), ERROR);
// 	}
// 	before = get_env_value(*(ast->shared_data->envs), "PWD");
// 	after = getcwd(NULL, PATH_MAX);
// 	ft_update_env_var("OLDPWD", before, *(ast->shared_data->envs));
// 	ft_update_env_var("OLDPWD", before, *(ast->shared_data->env_exp));
// 	ft_update_env_var("PWD", after, *(ast->shared_data->envs));
// 	ft_update_env_var("PWD", after, *(ast->shared_data->env_exp));
// 	free(before);
// 	free(after);
// 	ast->exit_status = 0;
// 	set_last_exit(0);
// 	return (SUCCESS);
// }

// bool	ft_do_step(char *to_go, char *cd_arg)
// {
// 	char	*before;

// 	before = getcwd(NULL, PATH_MAX);


// 	free(before);
// 	return (true);
// }

static void	ft_cd_alone(t_ast *ast)
{
	char	*to_go;
	char	*before;

	//? maybe i should/could check here whether HOME is unset.
	before = getcwd(NULL, PATH_MAX);
	to_go = get_env_value(*(ast->shared_data->envs), "HOME");
	if (chdir(to_go) < 0)
	{
		ft_cur_exit(ast, 1);
		print_error(true, "cd", NULL, strerror(errno));
		free(before);
		free(to_go);
		return ;
	}
	

	free(before);
}

// cd in bash cares only about the first argument. if there are more it ignores them.
void	ft_cd(t_ast *ast)
{
	char	**steps;
	char	*cd_arg;
	int		i;

	ft_cur_exit(ast, 0);
	cd_arg = ast->arg->name->token->str_data;
	printf("hello from cd\n");
	if (!cd_arg) //if there is no argument cd changes to HOME
	{
		ft_cd_alone(ast);
		return ;
	}
	if (*cd_arg == '\0') //cd doesn't do anything with empty argument
		return ;
	steps = ft_split(cd_arg, '/');
	if (!steps)
		return ;
	i = -1;
	// while (steps[++i])
	// {
	// 	ft_do_step(steps[i], cd_arg);
	// }
	
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

