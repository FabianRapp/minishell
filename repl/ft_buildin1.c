/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buildin1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 03:44:06 by frapp             #+#    #+#             */
/*   Updated: 2024/01/29 10:51:25 by frapp            ###   ########.fr       */
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

void	zero_rl(void)
{
	int		len;

	len = ft_strlen(SHELL_PROMPT);
	while (rl_line_buffer && *rl_line_buffer)
	{
		
	}
}

// see header file for weird stuff to keep in mind for implentation
void	ft_exit(t_ast *ast)
{
	//if (is main process)
		printf("exit\n");
	if (ast->arg && includes_non_num(ast->arg->name->token->str_data))
	{
		errno = (signed char)255;
		if (ast->arg && ast->arg->name)
		{
			//ft_fprintf(2, "%s: %s: %s: %s\n", SHELL_NAME, "exit", ast->arg->name->token->str_data, "numeric argument required");
			print_error(1, "exit", ast->arg->name->token->str_data, "numeric argument required");
		}
			//print_error(1, "exit", NULL, "numeric argument required");
		ast->exit_status = 255;
		ast->info = FINISHED;
		//if (is main process)
			main_cleanup(ast->cleanup_data);
		exit(255);
		return ;
	}
	if (count_args(ast, ARGS) > 1)
	{
		print_error(1, "exit", ast->arg->name->token->str_data, "too many arguments");
		ast->info = SYNTAX_ERROR;
		ast->exit_status = 1;
		return ;
	}
	ast->exit_status = 0;
	ast->info = FINISHED;
	//if (is main process)
		main_cleanup(ast->cleanup_data);
	exit(0);
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
	return ;
}
