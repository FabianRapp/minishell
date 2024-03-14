/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:47:46 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/14 05:54:28 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
exit [n]: is used to exit the current shell or script with a specified exit 
			status (n). if no numver is provided the exit will use the status 
			of the last executed command as its exit status (echo $?)
			
*/
//TODO: i need to add the -n


#include "../../headers/minishell.h"
#include "../../headers/eval.h"

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
