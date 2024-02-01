/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_ast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 12:08:53 by frapp             #+#    #+#             */
/*   Updated: 2024/02/01 16:07:08 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

bool	ft_pipe(t_ast *ast)
{
	if (ast->type != PIPE)
		return (false);
	printf("pipe placeholder\n");
	ast->exit_status = 0;
	return (true);
}

bool	run_subshell(t_ast *ast)
{
	t_ast	*sub_process;
	t_env	sub_env;

	sub_process = parser(ast->name->token->str_data);
	if (!sub_process)
	{
		// TODO: differ between syntax and malloc parser error
		ast->info = EXIT_ERROR;
		*(ast->env->last_exit_status) = 1;
		return (false);
	}
	sub_env = clone_env(ast->env);
	if (!sub_env.pid)
	{
		ast->info = EXIT_ERROR;
		*(ast->env->last_exit_status) = 1;
		return (false);
	}
	sub_process->env = &sub_env;
	sub_process->cleanup_data = ast->cleanup_data;
	walk_ast(sub_process);
	ast->exit_status = sub_process->exit_status;
	*(ast->env->last_exit_status) = ast->exit_status;
	ast->info = sub_process->info;
	if (ast->info == EXIT)
		ast->info = FINISHED;
	ast->cleanup_data->root->info = ast->info;
	free_ast(sub_process);
	//free sub env
	if (ast->info == EXIT_ERROR)
		return (false);
	return (true);
}

void	ft_and(t_ast *ast)
{
	walk_ast(ast->left);
	if (ast->left->exit_status)
	{
		ast->exit_status = ast->left->exit_status;
		return ;
	}
	walk_ast(ast->right);
	ast->exit_status = ast->right->exit_status;
	return ;
}

void	ft_or(t_ast *ast)
{
	walk_ast(ast->left);
	if (!ast->left->exit_status)
	{
		ast->exit_status = ast->left->exit_status;
		return ;
	}
	walk_ast(ast->right);
	ast->exit_status = ast->right->exit_status;
	return ;
}

// expects tree syntax to be ok on the token level
void	walk_ast(t_ast *ast)
{
	if (ast->info != EXIT)
	{
		if (ast->left)
		{
			ast->left->env = ast->env;
			ast->left->cleanup_data = ast->cleanup_data;
		}
		if (ast->right)
		{
			ast->right->env = ast->env;
			ast->right->cleanup_data = ast->cleanup_data;
		}
		if (ast->type == AND)
		{
			ft_and(ast);
		}
		else if (ast->type == OR)
		{
			ft_or(ast);
		}
		else if (ast->type == PIPE)
		{
			ft_pipe(ast);
		}
		else if (ast->name && ast->name->token->type == SUBSHELL)
		{
			if (!run_subshell(ast))
				return ;
		}
		else if (ast->type == COMMAND)
		{
			run_command_node(ast);
		}
	}
}
