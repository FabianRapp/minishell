/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_ast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 12:08:53 by frapp             #+#    #+#             */
/*   Updated: 2024/03/26 18:47:33 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static void	ft_or(t_ast *ast)
{
	bool	success_left;

	if (ast->exit_status != DEFAULT_EXIT_STATUS)
		ast->left->exit_status = ast->exit_status;
	else
	{
		run_node(ast->left);
		if (ast->left->exit_status == DEFAULT_EXIT_STATUS)
		{
			waitpid(ast->left->pid, &(ast->left->exit_status), 0);
			ast->left->exit_status = WEXITSTATUS(ast->left->exit_status);
		}
	}
	set_last_exit(ast->left->exit_status);
	success_left = !((bool)ast->left->exit_status);
	if (success_left)
		ast->right->exit_status = 0;
	run_node(ast->right);
	if (ast->right->exit_status == DEFAULT_EXIT_STATUS)
	{
		waitpid(ast->right->pid, &(ast->right->exit_status), 0);
		ast->right->exit_status = WEXITSTATUS(ast->right->exit_status);
	}
	set_last_exit(ast->right->exit_status);
	ast->exit_status = ast->right->exit_status;
}

static void	ft_and(t_ast *ast)
{
	bool	success_left;

	if (ast->exit_status != DEFAULT_EXIT_STATUS)
		ast->left->exit_status = ast->exit_status;
	else
	{
		run_node(ast->left);
		if (ast->left->exit_status == DEFAULT_EXIT_STATUS)
		{
			waitpid(ast->left->pid, &(ast->left->exit_status), 0);
			ast->left->exit_status = WEXITSTATUS(ast->left->exit_status);
		}
	}
	set_last_exit(ast->left->exit_status);
	success_left = !((bool)ast->left->exit_status);
	if (!success_left)
		ast->right->exit_status = ast->left->exit_status;
	run_node(ast->right);
	if (ast->right->exit_status == DEFAULT_EXIT_STATUS)
	{
		waitpid(ast->right->pid, &(ast->right->exit_status), 0);
		ast->right->exit_status = WEXITSTATUS(ast->right->exit_status);
	}
	set_last_exit(ast->right->exit_status);
	ast->exit_status = ast->right->exit_status;
}

static void	init_command(t_ast *ast)
{
	if (ast->shared_data->stop_execution)
	{
		ast->exit_status = 1;
		return ;
	}
	if (!ast->dont_run_buildins && !expansion(ast))
	{
		exit(1);
		return ;
	}
	if (resolve_redirs(ast) == ERROR)
	{
		cleanup_fds();
		return ;
	}
	if (ast->exit_status != DEFAULT_EXIT_STATUS)
		return ;
	redir_fds();
	run_command_node(ast);
	cleanup_fds();
	set_last_exit(ast->exit_status);
}

static void	ft_semicol(t_ast *ast)
{
	if (ast->left)
	{
		run_node(ast->left);
		if (ast->left->exit_status != DEFAULT_EXIT_STATUS)
			ast->exit_status = ast->left->exit_status;
		else
			ast->pid = ast->left->pid;
	}
	if (ast->right)
	{
		run_node(ast->right);
		if (ast->right->exit_status != DEFAULT_EXIT_STATUS)
			ast->exit_status = ast->right->exit_status;
		else
			ast->pid = ast->right->pid;
	}
}

void	run_node(t_ast *ast)
{
	errno = 0;
	if (!ast)
		return ;
	if (ast->type == AND)
		ft_and(ast);
	else if (ast->type == OR)
		ft_or(ast);
	else if (ast->type == SEMICOL)
		ft_semicol(ast);
	if (ast->exit_status == DEFAULT_EXIT_STATUS)
	{
		if (ast->type == PIPE)
			ft_pipe(ast);
		if (ast->name && ast->name->token->type == SUBSHELL)
			create_sub_shell(ast);
		else if (ast->type == COMMAND)
		{
			init_command(ast);
		}
	}
}
