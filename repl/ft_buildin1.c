/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buildin1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 03:44:06 by frapp             #+#    #+#             */
/*   Updated: 2024/02/25 07:01:48 by frapp            ###   ########.fr       */
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

void	ft_pwd(t_ast *ast)
{
	(void) ast;
	ft_printf("%s\n", getenv("PWD"));
}

void	ft_exit(t_ast *ast)
{
	if (ast->env->main_process)
		print_error(false, NULL, NULL, "exit");
	if (ast->arg && includes_non_num(ast->arg->name->token->str_data))
	{
		if (ast->arg && ast->arg->name)
			print_error(1, "exit", ast->arg->name->token->str_data, "numeric argument required");
		exit(255);
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
		exit(1);
	}
	else if (!ast->arg || count_args(ast->arg) == 0)
	{
		exit( 0);
	}
	else
	{
		if (ast->arg->name)
			exit(ft_atoi(ast->arg->name->token->str_data));
		else
			exit( 0);; // should not be needed later on
	}
}

typedef struct	s_cd
{
	char	cur_dir[PATH_MAX + 2];
	t_path	path_ob;
}	t_cd;


bool	ft_buildin(t_ast *ast)
{
	char	*command_name;

	command_name = ft_strdup(ast->name->token->str_data);
	if (!command_name)
	{
		ast->exit_status = 1;
		return (true);
	}
	ft_strtolower(command_name);
	//if (!ft_strcmp(command_name, "echo"))
		//return (ft_echo(ast), true);
	//if (!ft_strcmp(command_name, "cd"))
		//return (ft_cd(ast), true);
	if (!ft_strcmp(command_name, "pwd"))
		return (ft_pwd(ast), free(command_name), true);
	//if (!ft_strcmp(command_name, "export"))
		//return (ft_export(ast), true);
	//if (!ft_strcmp(command_name, "unset"))
		//return (ft_unset(ast), true);
	//if (!ft_strcmp(command_name, "env"))
		//return (ft_env(ast), true);
	if (!ft_strcmp(command_name, "exit"))
		return (ft_exit(ast), true);
	return (free(command_name), false);
}
