/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buildin1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 03:44:06 by frapp             #+#    #+#             */
/*   Updated: 2024/02/01 10:58:00 by frapp            ###   ########.fr       */
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

// see header file for weird stuff to keep in mind for implentation
void	ft_exit(t_ast *ast)
{
	int	exit_status;

	if (ast->env->main_process)
		printf("exit\n");
	if (ast->arg && includes_non_num(ast->arg->name->token->str_data))
	{
		if (ast->arg && ast->arg->name)
			print_error(1, "exit", ast->arg->name->token->str_data, "numeric argument required");
		ast->exit_status = 255;
		ast->info = FINISHED;
		
	}
	else if (ast->arg && count_args(ast, ARGS) > 1)
	{
		print_error(1, "exit", ast->arg->name->token->str_data, "too many arguments");
		ast->info = SYNTAX_ERROR;
		ast->exit_status = 1;
		if (ast->env->main_process)
			return ;
	}
	else if (!ast->arg || count_args(ast, ARGS) == 0)
	{
		ast->info = FINISHED;
		ast->exit_status = 0;
	}
	else
	{
		ast->info = FINISHED;
		if (ast->arg->name)
			ast->exit_status = ft_atoi(ast->arg->name->token->str_data);
		else
			ast->exit_status = 0; // should not be needed later on
	}
	exit_status = ast->exit_status;
	*(ast->env->last_exit_status) = ast->exit_status;
	if (ast->env->main_process)
	{
		main_cleanup(ast->cleanup_data, true, ast->env->main_process);
		exit(exit_status);
	}
	ast->info = EXIT;
}

typedef struct	s_cd
{
	char	cur_dir[PATH_MAX + 2];
	t_path	path_ob;
}	t_cd;

void	ft_cd(t_ast *ast)
{
	t_cd	cd_ob;

	init_path(&(cd_ob.path_ob), &(ast->info), "CDPATH");
	if (!getcwd(cd_ob.cur_dir, PATH_MAX + 1))
	{
		//error cur dir too long
		return ;
	}
	if (!ast->arg || !ast->arg->name)
		chdir(getenv("HOME"));
	else if (!ft_strcmp(ast->arg->name->token->str_data, "-"))
	{
		// TODO: change to last buffered dir
		ft_printf("last dir place holder\n");
	}
	my_free((void **)&(cd_ob.path_ob.cur_path));
}

//void	ft_export(t_ast *ast)
//{
	
//}

void	ft_buildin(t_ast *ast)
{
	char	*command_name;

	command_name = ast->name->token->str_data;
	// if (!ft_strcmp(command_name, "exit"))
	// {
	// 	ft_exit(ast);
	// 	return ;
	// }
	//if (!ft_strcmp(command_name, "echo"))
	{
	//	ft_exit(ast);
		//return ;
	}
	if (!ft_strcmp(command_name, "export"))
	{
		///ft_export(ast);
		return ;
	}
	return ;
}
