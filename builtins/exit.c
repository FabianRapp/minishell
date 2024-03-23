/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:47:46 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/23 07:03:16 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

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
	char	*tmp;

	if (sub_shell_mode(GET_SUB_SHELL_MODE) == false && !TESTER)
		print_error(false, NULL, NULL, "exit");
	if (!ast->arg || count_args(ast->arg) == 0)
		main_exit(ast->shared_data->cleanup_data, true, true);
	tmp = ast->arg->name->token->str_data;
	if (tmp && ((*tmp == '+' && *(tmp + 1)) || (*tmp == '-' && *(tmp + 1))))
		tmp++;
	if (tmp && includes_non_num(tmp))
	{
		print_error(true, "exit", ast->arg->name->token->str_data,
			"numeric argument required");
		ft_cur_exit(ast, 2);
	}
	else if (count_args(ast->arg) > 1)
	{
		print_error(true, "exit", NULL, "too many arguments");
		ft_cur_exit(ast, 1);
		ast->shared_data->stop_execution = true;
		return ;
	}
	else
		ft_cur_exit(ast, ft_atoi(ast->arg->name->token->str_data));
	main_exit(ast->shared_data->cleanup_data, true, true);
}

// THE VERY CONDENSED VERSION:
// int	ft_exit(t_ast *ast)
// {
// 	char	*tmp;

// 	if (sub_shell_mode(GET_SUB_SHELL_MODE) == false && !TESTER)
// 		print_error(false, NULL, NULL, "exit");
// 	if (!ast->arg || count_args(ast->arg) == 0)
// 		main_exit(ast->shared_data->cleanup_data, true, true);
	// tmp = NULL;
	// if (ast->arg->name)mste
	// 	tmp = ast->arg->name->token->str_data;
// 	if (tmp && ((*tmp == '+' && *(tmp + 1)) || (*tmp == '-' && *(tmp + 1))))
// 		tmp++;
// 	if (tmp && includes_non_num(tmp))
// 		return (print_error(true, "exit", ast->arg->name->token->str_data,
// 				"numeric argument required"), ft_cur_exit(ast, 2),
// 			main_exit(ast->shared_data->cleanup_data, true, true), 0);
// 	else if (count_args(ast->arg) > 1)
// 	{
// 		print_error(true, "exit", NULL, "too many arguments");
// 		ft_cur_exit(ast, 1);
// 		ast->shared_data->stop_execution = true;
// 		return (0);
// 	}
// 	ft_cur_exit(ast, ft_atoi(ast->arg->name->token->str_data));
// 	main_exit(ast->shared_data->cleanup_data, true, true);
// 	return (0);
// }
