/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:47:46 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/16 21:26:36 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
exit [n]: is used to exit the current shell or script with a specified exit 
			status (n). if no numver is provided the exit will use the status 
			of the last executed command as its exit status (echo $?)
			
*/



#include "../../headers/minishell.h"
#include "../../headers/eval.h"

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
