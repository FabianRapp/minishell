/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:26:07 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/27 18:51:23 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static void	ft_cd_to_var(t_ast *ast, char *var)
{
	char	to_go[PATH_MAX + 1];
	char	before[PATH_MAX + 1];
	char	after[PATH_MAX + 1];
	char	old_pwd[PATH_MAX + 1];

	getcwd(before, PATH_MAX);
	get_env_value(*(ast->shared_data->envs), var, to_go, PATH_MAX);
	if (chdir(to_go) < 0)
	{
		get_env_value(NULL, "OLDPWD", old_pwd, PATH_MAX);
		chdir(old_pwd);
		ft_cur_exit(ast, 1);
		print_error(true, "cd", NULL, strerror(errno));
		return ;
	}
	getcwd(after, PATH_MAX);
	ft_update_dir_vars(ast, before, after);
	if (!ft_strcmp(var, "OLDPWD"))
		ft_pwd(ast);
}

static int	ft_cd_step(t_cd_step_data data)
{
	char	*tmp;

	tmp = init_ft_cd_step(data.ast, data.step, data.index);
	if (!tmp)
		return (0);
	if (check_path_len(data.ast, tmp) == ERROR)
		return (free(tmp), 0);
	ft_strlcpy(data.to_go, tmp, PATH_MAX);
	free(tmp);
	if (ft_strlen(data.to_go) == 0)
		ft_strlcpy(data.to_go, "/", PATH_MAX);
	get_env_value(NULL, "OLDPWD", data.old_pwd, PATH_MAX);
	if (chdir(data.to_go) < 0)
		return (chdir(data.old_pwd), ft_cur_exit(data.ast, 1),
			print_error(true, "cd",
				data.cd_arg, strerror(errno)), 0);
	getcwd(data.after, PATH_MAX);
	if (data.first)
		ft_update_dir_vars(data.ast, data.before, data.after);
	return (1);
}

static t_arg	*cd_edge_cases(t_ast *ast)
{
	t_arg	*args;
	char	*cd_arg;

	ft_cur_exit(ast, 0);
	args = ast->arg;
	if (!args)
		return (ft_cd_to_var(ast, "HOME"), NULL);
	if (count_args(ast->arg) > 1)
		return (ft_cur_exit(ast, 1), print_error(true,
				"cd", NULL, "too many arguments"), NULL);
	cd_arg = ast->arg->name->token->str_data;
	if (*cd_arg == '\0')
		return (NULL);
	if (ft_strcmp(cd_arg, "-") == 0)
		return (ft_cd_to_var(ast, "OLDPWD"), NULL);
	if (ft_strcmp(cd_arg, "--") == 0)
		return (ft_cd_to_var(ast, "HOME"), NULL);
	if (check_path_len(ast, cd_arg) == ERROR)
		return (NULL);
	return (args);
}

static bool	cd_loop(t_cd_vars vars, t_cd_step_data step_data)
{
	if (!vars.steps)
	{
		return (chdir(vars.pwd_before),
			ft_update_env("OLDPWD", vars.old_pwd,
				*(step_data.ast->shared_data->envs)), 0);
	}
	while (vars.steps[++(vars.i)])
	{
		step_data.step = vars.steps[vars.i];
		step_data.index = vars.i;
		step_data.first = vars.i == 0;
		if (!ft_cd_step(step_data))
		{
			return (chdir(vars.pwd_before),
				ft_update_env("OLDPWD", vars.old_pwd,
					*(step_data.ast->shared_data->envs)), 0);
		}
	}
	return (1);
}

int	ft_cd(t_ast *ast)
{
	t_cd_vars		vars;
	t_cd_step_data	step_data;

	getcwd(step_data.before, PATH_MAX);
	step_data.ast = ast;
	if (!cd_edge_cases(ast))
		return (0);
	vars.cd_arg = ast->arg->name->token->str_data;
	get_env_value(*(ast->shared_data->envs), "OLDPWD", vars.old_pwd, PATH_MAX);
	get_env_value(*(ast->shared_data->envs), "PWD", vars.pwd_before, PATH_MAX);
	if (errno)
		return (set_errno_as_exit(ast, false));
	vars.i = -1;
	ft_update_env("OLDPWD", vars.pwd_before, *(ast->shared_data->envs));
	step_data.step = "/";
	step_data.index = 0;
	step_data.cd_arg = vars.cd_arg;
	step_data.first = true;
	if (*(vars.cd_arg) == '/' && !ft_cd_step(step_data))
		return (chdir(vars.pwd_before), ft_update_env("OLDPWD",
				vars.old_pwd, *(ast->shared_data->envs)), 0);
	vars.steps = ft_split(vars.cd_arg, '/');
	cd_loop(vars, step_data);
	ft_free_2darr(vars.steps);
	return (1);
}
