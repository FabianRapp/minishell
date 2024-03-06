/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buildin1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 03:44:06 by frapp             #+#    #+#             */
/*   Updated: 2024/03/06 08:18:57 by frapp            ###   ########.fr       */
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
	if (sub_shell_mode(GET_SUB_SHELL_MODE) == false)
		print_error(false, NULL, NULL, "exit");
	if (ast->arg && includes_non_num(ast->arg->name->token->str_data))
	{
		if (ast->arg && ast->arg->name)
			print_error(true, "exit", ast->arg->name->token->str_data, "numeric argument required");
		set_last_exit(255);
		main_exit(ast->cleanup_data, true);
	}
	else if (ast->arg && count_args(ast->arg) > 1)
	{
		print_error(true, "exit", NULL, "too many arguments");
		ast->exit_status = 1;
		set_last_exit(1);
		ast->env->stop_execution = true;
		return ;
	}
	else if (!ast->arg || count_args(ast->arg) == 0)
		main_exit(ast->cleanup_data, true);
	else
	{
		set_last_exit(ft_atoi(ast->arg->name->token->str_data));
		main_exit(ast->cleanup_data, true);
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
		//return (free(command_name), ft_echo(ast), true);
	//if (!ft_strcmp(command_name, "cd"))
		//return (free(command_name), ft_cd(ast), true);
	if (!ft_strcmp(command_name, "pwd"))
		return (free(command_name), ft_pwd(ast), true);
	//if (!ft_strcmp(command_name, "export"))
		//return (free(command_name),ft_export(ast), true);
	//if (!ft_strcmp(command_name, "unset"))
		//return (free(command_name),ft_unset(ast), true);
	//if (!ft_strcmp(command_name, "env"))
		//return (free(command_name), ft_env(ast), true);
	if (!ft_strcmp(command_name, "exit"))
		return (free(command_name), ft_exit(ast), true);
	return (free(command_name), false);
}
