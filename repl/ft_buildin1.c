/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buildin1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 03:44:06 by frapp             #+#    #+#             */
/*   Updated: 2024/02/14 14:41:32 by frapp            ###   ########.fr       */
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

// see header file for weird stuff to keep in mind for implentation
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
			return ;
		exit(1);
	}
	else if (!ast->arg || count_args(ast->arg) == 0)
	{
		exit( 0);
	}
	else
	{
		if (ast->arg->name)
			exit( ft_atoi(ast->arg->name->token->str_data));
		else
			exit( 0);; // should not be needed later on
	}
	printf("debug exit, why is it getting here\n");
	//if (ast->env->main_process)
	{
		
		//exit( exit_status);

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

	if (ast->name->token->str_data && !ft_strcmp(ast->name->token->str_data, "exit"))
	{
		resolve_redirs(ast);
		ft_exit(ast);
		return (true);
	}
	command_name = ast->name->token->str_data;
	//if (!ft_strcmp(command_name, "echo"))
	{
	//	ft_exit(ast);
		//return ;
	}
	if (!ft_strcmp(command_name, "export"))
	{
		///ft_export(ast);
		return (true);
	}
	else if (!ft_strcmp(command_name, "pwd"))
	{
		ft_pwd(ast);
		return (true);
	}
	return (false);
}
