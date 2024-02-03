/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buildin1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 03:44:06 by frapp             #+#    #+#             */
/*   Updated: 2024/02/03 18:19:59 by frapp            ###   ########.fr       */
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
		print_error(false, NULL, NULL, "exit");
	if (ast->arg && includes_non_num(ast->arg->name->token->str_data))
	{
		if (ast->arg && ast->arg->name)
			print_error(1, "exit", ast->arg->name->token->str_data, "numeric argument required");
		my_exit(ast, 255);
		exit(255);
	}
	else if (ast->arg && count_args(ast->arg) > 1)
	{
		print_error(1, "exit", ast->arg->name->token->str_data, "too many arguments");
		exit(1);
		if (ast->env->main_process)
			return ;
	}
	else if (!ast->arg || count_args(ast->arg) == 0)
	{
		my_exit(ast, 0);
	}
	else
	{
		
		if (ast->arg->name)
			my_exit(ast, ft_atoi(ast->arg->name->token->str_data));
		else
			my_exit(ast, 0);; // should not be needed later on
	}
	printf("debug exit, why is it getting here\n");
	exit_status = ast->env->exit_status;
	//if (ast->env->main_process)
	{
		
		my_exit(ast, exit_status);
		// close(ast->fd[1]);
		// close(ast->fd[0]);
		// main_exit(ast->cleanup_data, true, ast->env, exit_status);
	}
}

typedef struct	s_cd
{
	char	cur_dir[PATH_MAX + 2];
	t_path	path_ob;
}	t_cd;

// void	ft_cd(t_ast *ast)
// {
// 	t_cd	cd_ob;

// 	init_path(&(cd_ob.path_ob), &(ast->info), "CDPATH");
// 	if (!getcwd(cd_ob.cur_dir, PATH_MAX + 1))
// 	{
// 		//error cur dir too long
// 		return ;
// 	}
// 	if (!ast->arg || !ast->arg->name)
// 		chdir(getenv("HOME"));
// 	else if (!ft_strcmp(ast->arg->name->token->str_data, "-"))
// 	{
// 		// TODO: change to last buffered dir
// 		ft_printf("last dir place holder\n");
// 	}
// 	my_free((void **)&(cd_ob.path_ob.cur_path));
// }

//void	ft_export(t_ast *ast)
//{
	
//}

bool	ft_buildin(t_ast *ast)
{
	char	*command_name;

	if (ast->name->token->str_data && !ft_strcmp(ast->name->token->str_data, "exit"))
	{
		resolve_redirs(ast);
		ft_exit(ast);
		reset_stdio(ast);
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
	return (false);
}
