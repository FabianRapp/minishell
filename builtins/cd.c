/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:26:07 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/22 19:38:22 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/eval.h"

static void	ft_cd_to_var(t_ast *ast, bool not_alone, char *path, char *var)
{
	char	*to_go;
	char	*before;
	char	*after;
	char	*old_pwd;

	before = getcwd(NULL, PATH_MAX);
	if (ft_strcmp(var, "HOME") && ft_strcmp(var, "PWD")
		&& ft_strcmp(var, "OLDPWD"))
		to_go = var;
	else
		to_go = get_env_value(*(ast->shared_data->envs), var);
	if (chdir(to_go) < 0)
	{
		old_pwd = get_env_value(NULL, "OLDPWD");
		(chdir(old_pwd), free(old_pwd), ft_cur_exit(ast, 1));
		if (not_alone)
			print_error(true, "cd", path, "No such file or directory");
		else
			print_error(true, "cd", NULL, strerror(errno));
		(free(before), free(to_go));
		return ;
	}
	after = getcwd(NULL, PATH_MAX);
	(ft_update_dir_vars(ast, before, after),
		(ft_strcmp(var, "OLDPWD") || ft_pwd(ast)), free(to_go));
}

static int	ft_cd_step(t_cd_step_data data)
{
	data.to_go = init_ft_cd_step(data.ast, data.step, data.index);
	if (!data.to_go)
		return (0);
	data.before = getcwd(NULL, PATH_MAX);
	if (data.to_go && ft_strlen(data.to_go) == 0)
	{
		free(data.to_go);
		data.to_go = ft_strdup("/");
		if (!data.to_go)
			return (set_errno_as_exit(data.ast, false), 0);
	}
	data.old_pwd = get_env_value(NULL, "OLDPWD");
	if (!data.old_pwd)
		return (ft_cur_exit(data.ast, 1), free(data.to_go),
			free(data.before), set_errno_as_exit(data.ast, false));
	if (chdir(data.to_go) < 0)
		return (chdir(data.old_pwd), free(data.old_pwd),
			ft_cur_exit(data.ast, 1), free(data.to_go), free(data.before),
			print_error(true, "cd", data.cd_arg,
				"No such file or directory"), 0);
	free(data.old_pwd);
	data.after = getcwd(NULL, PATH_MAX);
	return ((data.first && ft_update_dir_vars(data.ast,
				data.before, data.after)), 1);
}

static t_arg	*cd_edge_cases(t_ast *ast)
{
	t_arg	*args;
	char	*cd_arg;

	(ft_cur_exit(ast, 0), args = ast->arg);
	if (!args)
		return (ft_cd_to_var(ast, false, NULL, "HOME"), NULL);
	if (count_args(ast->arg) > 1)
		return (ft_cur_exit(ast, 1), print_error(true, "cd", NULL,
				"too many arguments"), NULL);
	cd_arg = ast->arg->name->token->str_data;
	if (*cd_arg == '\0')
		return (NULL);
	if (ft_strcmp(cd_arg, "-") == 0)
		return (ft_cd_to_var(ast, false, NULL, "OLDPWD"), NULL);
	if (ft_strcmp(cd_arg, "--") == 0)
		return (ft_cd_to_var(ast, false, NULL, "HOME"), NULL);
	return (args);
}

static bool	cd_loop(t_cd_vars vars, t_cd_step_data step_data)
{
	if (!vars.steps)
		return (chdir(vars.tmp), free(vars.tmp), ft_update_env("OLDPWD",
				vars.old_pwd, *(step_data.ast->shared_data->envs)),
			free(vars.old_pwd), 0);
	while (vars.steps[++(vars.i)])
	{
		step_data.step = vars.steps[vars.i];
		step_data.index = vars.i;
		step_data.first = vars.i == 0;
		if (!ft_cd_step(step_data))
			return (chdir(vars.tmp), free(vars.tmp),
				ft_update_env("OLDPWD", vars.old_pwd,
					*(step_data.ast->shared_data->envs)),
				free(vars.old_pwd), 0);
	}
	return (free(vars.tmp), free(vars.old_pwd), ft_free_2darr(vars.steps), 1);
}

int	ft_cd(t_ast *ast)
{
	t_cd_vars		vars;
	t_cd_step_data	step_data;

	step_data.ast = ast;
	if (!cd_edge_cases(ast))
		return (0);
	vars.cd_arg = ast->arg->name->token->str_data;
	vars.old_pwd = get_env_value(*(ast->shared_data->envs), "OLDPWD");
	vars.tmp = get_env_value(*(ast->shared_data->envs), "PWD");
	vars.i = -1;
	ft_update_env("OLDPWD", vars.tmp, *(ast->shared_data->envs));
	step_data.step = "/";
	step_data.index = 0;
	step_data.cd_arg = vars.cd_arg;
	step_data.first = true;
	if (*(vars.cd_arg) == '/' && !ft_cd_step(step_data))
		return (chdir(vars.tmp), free(vars.tmp), ft_update_env("OLDPWD",
				vars.old_pwd, *(ast->shared_data->envs)),
			free(vars.old_pwd), 0);
	if (*(vars.cd_arg) == '*')
		(free(vars.cd_arg),
			vars.cd_arg = get_env_value(*(ast->shared_data->envs), "HOME"));
	vars.steps = ft_split(vars.cd_arg, '/');
	return (cd_loop(vars, step_data));
}
