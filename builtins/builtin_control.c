/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_control.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:34:24 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/25 07:28:44 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

bool	ft_builtin_control(t_ast *ast)
{
	char	*cmd_name;

	if (!ft_strcmp(ast->name->token->str_data, "exit"))
		return (ft_exit(ast), true);
	if (!ft_strcmp(ast->name->token->str_data, "cd"))
		return (ft_cd(ast), true);
	if (!ft_strcmp(ast->name->token->str_data, "export"))
		return (ft_export(ast, ast->arg), true);
	if (!ft_strcmp(ast->name->token->str_data, "unset"))
		return (ft_unset(ast), true);
	if (!ft_strcmp(ast->name->token->str_data, "echo"))
		return (ft_echo(ast, ast->arg), true);
	cmd_name = ft_strdup(ast->name->token->str_data);
	if (!cmd_name)
		return (ft_cur_exit(ast, 1), true);
	ft_strtolower(cmd_name);
	if (!ft_strcmp(cmd_name, "echo"))
		return (free(cmd_name), ft_cap_echo(ast, ast->arg), true);
	if (!ft_strcmp(cmd_name, "pwd"))
		return (free(cmd_name), ft_pwd(ast), true);
	if (!ft_strcmp(cmd_name, "env"))
		return (free(cmd_name), ft_env(ast), true);
	return (free(cmd_name), false);
}
