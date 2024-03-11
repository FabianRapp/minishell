/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buildin1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 03:44:06 by frapp             #+#    #+#             */
/*   Updated: 2024/03/11 01:50:28 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/eval.h"

static bool	includes_non_num(char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (true);
		str++;
	}
	return (false);
}

void	ft_exit(t_ast *ast)
{
	if (ast->env->main_process)
		print_error(false, NULL, NULL, "exit");
	if (ast->arg && includes_non_num(ast->arg->name->token->str_data))
	{
		if (ast->arg && ast->arg->name)
			print_error(1, "exit", ast->arg->name->token->str_data, "numeric argument required");
		main_exit(ast->cleanup_data, true, ast->env, 255);
	}
	else if (ast->arg && count_args(ast->arg) > 1)
	{
		print_error(1, "exit", ast->arg->name->token->str_data, "too many arguments");
		ast->exit_status = 1;
		if (ast->env->main_process)
		{
			ast->env->stop_execution = true;
			return ;
		}
		main_exit(ast->cleanup_data, true, ast->env, 1);
	}
	else if (!ast->arg || count_args(ast->arg) == 0)
		main_exit(ast->cleanup_data, true, ast->env, 0);
	else
		main_exit(ast->cleanup_data, true, ast->env, ft_atoi(ast->arg->name->token->str_data));
}

typedef struct	s_cd
{
	char	cur_dir[PATH_MAX + 2];
	t_path	path_ob;
}	t_cd;


bool	ft_buildin(t_ast *ast)
{
	char	*cmd_name;

	cmd_name = ft_strdup(ast->name->token->str_data); //? why dynamically allocated and not just copy the ptr?
	// cmd_name = ast->name->token->str_data; //since we don't want to do modifications but just read
	if (!cmd_name)
	{
		ast->exit_status = 1;
		return (true);
	}
	ft_strtolower(cmd_name);
	if (!ft_strcmp(cmd_name, "exit"))
		return (free(cmd_name), ft_exit(ast), true);
	//if (!ft_strcmp(cmd_name, "echo"))
		//return (free(cmd_name), ft_echo(ast), true);
	// if (!ft_strcmp(cmd_name, "cd"))
	// 	return (free(cmd_name), ft_cd(ast), true);
	if (!ft_strcmp(cmd_name, "pwd"))
		return (free(cmd_name), ft_pwd(), true);
	if (!ft_strcmp(cmd_name, "export"))
		return (free(cmd_name), ft_export(ast), true);
	//if (!ft_strcmp(cmd_name, "unset"))
		//return (free(cmd_name),ft_unset(ast), true);
	if (!ft_strcmp(cmd_name, "env"))
		return (free(cmd_name), ft_env(ast), true);
	return (false);
}
