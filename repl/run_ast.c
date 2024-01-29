/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_ast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 12:08:53 by frapp             #+#    #+#             */
/*   Updated: 2024/01/29 13:31:16 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

bool	left_branching(t_ast *ast)
{
	if (ast && ast->left)
	{
		walk_ast (ast->left, ast->cleanup_data, ast->main_process);
		ast->info = ast->left->info;
		ast->exit_status = ast->left->exit_status;
		ast->cleanup_data->root->exit_status = ast->exit_status;
		ast->cleanup_data->root->info = ast->left->info;
		if (ast->left->info == EXIT_ERROR)
		{
			return (false);
		}
		if (ast->left->info == FALSE)
		{
			ft_printf("debugging walk_ast\n");
			
			ast->cleanup_data->root->info = EXIT_ERROR;
			return (false);
		}
	}
	return (true);
}

bool	right_branching(t_ast *ast)
{
	if (ast && ast->right)
	{
		if ((ast->type == AND && ast->left->exit_status == 0) || (ast->type == OR && ast->left->exit_status != 0))
		//TODO implement PIPES
		{
			walk_ast(ast->right, ast->cleanup_data, ast->main_process);
			ast->info = ast->right->info;
			ast->exit_status = ast->right->exit_status;
			ast->cleanup_data->root->info = ast->right->info;
			ast->cleanup_data->root->exit_status = ast->exit_status;
			if (ast->right->info == EXIT_ERROR)
			{
				return (false);
			}
			if (ast->left->info == FALSE)
			{
				ft_printf("debugging walk_ast\n");
				ast->cleanup_data->root->info = EXIT_ERROR;
				return (false);
			}
		}
	}
	return (true);
}

void	walk_ast(t_ast *ast, t_cleanup_data *cleanup_data, bool main_process)
{
	t_ast	*sub_process;

	ast->main_process = main_process;
	ast->cleanup_data = cleanup_data;
	if (ast->left)
	{
		left_branching(ast);
		if (ast->left->exit_status == EXIT_ERROR)
			return ;
	}
	if (ast->right)
	{
		right_branching(ast);
		if (ast->right->exit_status == EXIT_ERROR)
			return ;
	}
	else if (ast->name->token->type == SUBSHELL)
	{
		sub_process = parser(ast->name->token->str_data);
		if (sub_process)
		{
			walk_ast(sub_process, ast->cleanup_data, false);
			ast->exit_status = sub_process->exit_status;
			ast->info = sub_process->info;
			ast->cleanup_data->root->info = ast->info;
			ast->cleanup_data->root->exit_status = ast->exit_status;
			free_ast(sub_process);
			if (ast->info == EXIT_ERROR || ast->info == SYNTAX_ERROR)
				return ;
		}
		else
		{
			// TODO hadnle syntax parser errors
		}
	}
	else if (ast->type == COMMAND)
		run_command_node(ast);
}
