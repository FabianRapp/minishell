/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buildin1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 03:44:06 by frapp             #+#    #+#             */
/*   Updated: 2024/01/28 04:26:04 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

// see header file for weird stuff to keep in mind for implentation
int	ft_exit(t_ast *ast)
{
	// do redirs, if input redir fails return something to indicate no exit
	/*
	if (ast->arg && check_for_chars(ast->arg->name->token->str_data) // only check first arg otherwise second error
	{
		printf(SHELL_NAME ": exit: <first arg with chars>: numeric argument required\n");
		return (1);
	}
	*/

	if (count_args(ast, ARGS) > 1)
	{
		printf(SHELL_NAME ": exit: too many arguments\n");
		cleanup();
		exit(1);
	}
	cleanup();
	exit(0);
	return (1);//incase no exit to indicate ft function was found
}


int	ft_buildin(t_ast *ast)
{
	char	*command_name;

	command_name = ast->name->token->str_data;
	if (!ft_strcmp(command_name, "exit"))
		return(ft_exit(ast));
	return (0);
}
