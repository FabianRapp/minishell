/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_ast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 12:08:53 by frapp             #+#    #+#             */
/*   Updated: 2024/02/01 10:41:57 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

bool	and_or(t_ast *ast)
{
	if (ast->type != AND && ast->type != OR)
		return (false);
	if (!(ast->left && ast->right))
		return (printf("error and_or_right: missing child, syntax error should be found in parser\n"), exit(1), true);
	if ((ast->type == AND && ast->left->exit_status == 0) || (ast->type == OR && ast->left->exit_status != 0))
	{
		walk_ast(ast->right, ast->cleanup_data);
		ast->info = ast->right->info;
		ast->exit_status = ast->right->exit_status;
		*(ast->env->last_exit_status) = ast->exit_status;
		ast->cleanup_data->root->info = ast->right->info;
		ast->cleanup_data->root->exit_status = ast->exit_status;
		if (ast->right->info == EXIT_ERROR)
		{
			return (false);
		}
		if (ast->left->info == FALSE)
		{
			ft_printf("debugging: and_or\n");
			ast->cleanup_data->root->info = EXIT_ERROR;
			return (false);
		}
	}
	return (true);
}

bool	ft_pipe(t_ast *ast)
{
	if (ast->type != PIPE)
		return (false);
	printf("pipe placeholder\n");
	return (true);
}

bool	right_branching(t_ast *ast)
{
	if (!(ast && ast->right && ast->right->info == NOT_FINISHED))
		return (false);
	ast->right->env = ast->env;
	if (and_or(ast))
	{
	}
	else if (ft_pipe(ast))
	{
	}
	else
	{
		printf("error right_branching: unknown opperator: %s\n", type_to_str_type(ast->type));
		exit(1);
	}
	return (true);
}

bool	left_branching(t_ast *ast)
{
	if (!(ast && ast->left && ast->left->info == NOT_FINISHED))
		return (false);
	ast->left->env = ast->env;
	walk_ast (ast->left, ast->cleanup_data);
	ast->info = ast->left->info;
	ast->exit_status = ast->left->exit_status;
	*(ast->env->last_exit_status) = ast->exit_status;
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
	walk_ast(sub_process, ast->cleanup_data);
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

void	walk_ast(t_ast *ast, t_cleanup_data *cleanup_data)
{
	ast->cleanup_data = cleanup_data;
	if (ast->info != EXIT)
	{
		if (ast->left && ast->left->info == NOT_FINISHED)
		{
			left_branching(ast);
			if (ast->left->exit_status == EXIT_ERROR || ast->left->info == EXIT)
			{
				ast->info = ast->left->info;
				ast->exit_status = ast->left->exit_status;
				return ;
			}
		}
		if (ast->right && ast->right->info == NOT_FINISHED)
		{
			right_branching(ast);
			if (ast->right->exit_status == EXIT_ERROR)
				return ;
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
