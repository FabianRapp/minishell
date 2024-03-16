/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buildin1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 03:44:06 by frapp             #+#    #+#             */
/*   Updated: 2024/03/16 19:26:06 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/eval.h"

bool	ft_buildin(t_ast *ast)
{
	char	*cmd_name;

	cmd_name = ft_strdup(ast->name->token->str_data);
	if (!cmd_name)
	{
		ast->exit_status = 1;
		return (true);
	}
	ft_strtolower(cmd_name);
	if (!ft_strcmp(ast->name->token->str_data, "exit"))
		return (free(cmd_name), ft_exit(ast), true);
	if (!ft_strcmp(cmd_name, "echo"))
		return (free(cmd_name), ft_echo(ast), true);
	if (!ft_strcmp(cmd_name, "cd"))
		return (free(cmd_name), ft_cd(ast), true);
	if (!ft_strcmp(cmd_name, "pwd"))
		return (free(cmd_name), ft_pwd(ast), true);
	if (!ft_strcmp(cmd_name, "export"))
		return (free(cmd_name), ft_export(ast), true);
	if (!ft_strcmp(cmd_name, "unset"))
		return (free(cmd_name), ft_unset(ast), true);
	if (!ft_strcmp(cmd_name, "env"))
		return (free(cmd_name), ft_env(ast), true);
	return (false);
}
