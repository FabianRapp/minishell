/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buildin1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 03:44:06 by frapp             #+#    #+#             */
/*   Updated: 2024/03/18 05:06:33 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/eval.h"

bool	ft_buildin(t_ast *ast)
{
	char	*cmd_name;

	if (!ft_strcmp(ast->name->token->str_data, "exit"))
		return (ft_exit(ast), true);
	if (!ft_strcmp(ast->name->token->str_data, "cd"))
		return (ft_cd(ast), true);
	if (!ft_strcmp(ast->name->token->str_data, "export"))
		return (ft_export(ast), true);
	if (!ft_strcmp(ast->name->token->str_data, "unset"))
		return (ft_unset(ast), true);
	cmd_name = ft_strdup(ast->name->token->str_data);
	if (!cmd_name)
	{
		ast->exit_status = 1;
		return (true);
	}
	ft_strtolower(cmd_name);
	if (!ft_strcmp(cmd_name, "echo"))
		return (free(cmd_name), ft_echo(ast), true);
	if (!ft_strcmp(cmd_name, "pwd"))
		return (free(cmd_name), ft_pwd(ast), true);
	if (!ft_strcmp(cmd_name, "env"))
		return (free(cmd_name), ft_env(ast), true);
	return (false);
}

void	ft_cur_exit(t_ast *ast, int exit_value)
{
	ast->exit_status = exit_value;
	set_last_exit(exit_value);
}

//88, 24, 84 -> 196
