/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buildin1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 03:44:06 by frapp             #+#    #+#             */
/*   Updated: 2024/01/29 13:20:19 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

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

	if (ast->main_process)
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
	if (ast->main_process)
		main_cleanup(ast->cleanup_data);
	exit(exit_status);
}

void	ft_buildin(t_ast *ast)
{
	char	*command_name;

	command_name = ast->name->token->str_data;
	if (!ft_strcmp(command_name, "exit"))
	{
		ft_exit(ast);
		return ;
	}
	//if (!ft_strcmp(command_name, "echo"))
	{
	//	ft_exit(ast);
		//return ;
	}
	return ;
}
