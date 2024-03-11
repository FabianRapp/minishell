/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buildin1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 03:44:06 by frapp             #+#    #+#             */
/*   Updated: 2024/03/11 11:40:33 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/eval.h"

static bool	includes_non_num(char *str)
{
	bool	had_digit;

	had_digit = false;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (true);
		had_digit = true;
		str++;
	}
	return (!had_digit);
}

void	ft_pwd(t_ast *ast)
{
	(void) ast;
	ft_printf("%s\n", getenv("PWD"));
}

void	ft_exit(t_ast *ast)
{

	if (sub_shell_mode(GET_SUB_SHELL_MODE) == false && !TESTER)
		print_error(false, NULL, NULL, "exit");
	if (ast->arg && includes_non_num(ast->arg->name->token->str_data))
	{
		if (ast->arg && ast->arg->name && !TESTER)
			print_error(true, "exit", ast->arg->name->token->str_data, "numeric argument required");
		else if(ast->arg && ast->arg->name)
			ft_fprintf(2, "%s: %s: %s: %s\n", SHELL_NAME, "exit", ast->arg->name->token->str_data, "numeric argument required");
		set_last_exit(2);
		main_exit(ast->cleanup_data, true, true);
	}
	else if (ast->arg && count_args(ast->arg) > 1)
	{
		if (!TESTER)
			print_error(true, "exit", NULL, "too many arguments");
		else
			ft_fprintf(2, "%s: %s: %s\n", SHELL_NAME, "exit", "too many arguments");
		ast->exit_status = 1;
		set_last_exit(1);
		ast->env->stop_execution = true;
		return ;
	}
	else if (!ast->arg || count_args(ast->arg) == 0)
	{
		main_exit(ast->cleanup_data, true, true);
	}
	else
	{
		set_last_exit(ft_atoi(ast->arg->name->token->str_data));
		ast->exit_status = ft_atoi(ast->arg->name->token->str_data);
		// if (sub_shell_mode(GET_SUB_SHELL_MODE) == true)
		// 	printf("exit in sub mode: %d/%d\n", ast->exit_status, get_last_exit());
		// else
		// 	printf("exit not sub mode: %d/%d\n", ast->exit_status, get_last_exit());
		main_exit(ast->cleanup_data, true, true);
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
	if (!ft_strcmp(ast->name->token->str_data, "exit"))
		return (free(command_name), ft_exit(ast), true);
	return (free(command_name), false);
}
