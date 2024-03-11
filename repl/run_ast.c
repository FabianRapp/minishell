/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_ast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 12:08:53 by frapp             #+#    #+#             */
/*   Updated: 2024/03/11 12:13:42 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

// TODO errors
void	ft_pipe(t_ast *ast)
{
	int			pipe_fd[2];
	int			base;

	if (ast->env->stop_execution)
	{
		ast->exit_status = 1;
		return ;
	}
	//check_fds();
	base = dup(WRITE);
	if (base == -1)
	{
		set_errno_as_exit(ast, true);
		return ;
	}
	if (pipe(pipe_fd) == -1)
	{
		close (base);
		set_errno_as_exit(ast, true);
		return ;
	}
	if (dup2(pipe_fd[WRITE], WRITE) == -1)
	{
		close (base);
		close (pipe_fd[WRITE]);
		close (pipe_fd[READ]);
		set_errno_as_exit(ast, true);
		return ;
	}
	close(pipe_fd[WRITE]);
	ast->left->fd_to_close = pipe_fd[READ];
	run_node(ast->left);
	if (dup2(base, WRITE) == -1)
	{
		close (base);
		close (pipe_fd[READ]);
		set_errno_as_exit(ast, true);
		return ;
	}
	close(base);
	ast->env->stop_execution = false;
	base = dup(READ);
	if (base == -1)
	{
		close(pipe_fd[READ]);
		set_errno_as_exit(ast, true);
		return ;
	}
	if (dup2(pipe_fd[READ], READ) == -1)
	{
		close(base);
		close(pipe_fd[READ]);
		set_errno_as_exit(ast, true);
		return ;
	}
	close(pipe_fd[READ]);
	run_node(ast->right);
	if (dup2(base, READ) == -1)
	{
		close(base);
		set_errno_as_exit(ast, true);
		return ;
	}
	close(base);
	if (ast->right->exit_status == DEFAULT_EXIT_STATUS)
		ast->pid = ast->right->pid;
	else
		ast->exit_status = ast->right->exit_status;
	//check_fds();
}

void	ft_or(t_ast *ast)
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
	set_last_exit(ast->left->exit_status);//TODO mb out this behind a condtion so it only updates for process nodes not operators
	success_left = !((bool)ast->left->exit_status);
	if (success_left)
		ast->right->exit_status = 0;
	run_node(ast->right);
	if (ast->right->exit_status == DEFAULT_EXIT_STATUS)
	{
		waitpid(ast->right->pid, &(ast->right->exit_status), 0);
		ast->right->exit_status = WEXITSTATUS(ast->right->exit_status);
	}
	set_last_exit(ast->right->exit_status);//TODO mb out this behind a condtion so it only updates for process nodes not operators
	ast->exit_status = ast->right->exit_status;
}

// kinda workarround for wrong build ast for multiple condtions without subshell
void	ft_and(t_ast *ast)
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
	set_last_exit(ast->left->exit_status);//TODO mb out this behind a condtion so it only updates for process nodes not operators
	success_left = !((bool)ast->left->exit_status);
	if (!success_left)
		ast->right->exit_status = ast->left->exit_status;
	run_node(ast->right);
	if (ast->right->exit_status == DEFAULT_EXIT_STATUS)
	{
		waitpid(ast->right->pid, &(ast->right->exit_status), 0);
		ast->right->exit_status = WEXITSTATUS(ast->right->exit_status);
	}
	set_last_exit(ast->right->exit_status);//TODO mb out this behind a condtion so it only updates for process nodes not operators
	ast->exit_status = ast->right->exit_status;
}

void	init_command(t_ast *ast)
{
	if (ast->env->stop_execution)
	{
		ast->exit_status = 1;
		return ;
	}
	if (!expansion(ast))
	{
		//printf("DEBUG create_sub\n");
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

void	run_node(t_ast *ast)
{
	errno = 0;
	if (!ast)
		return ;
	if (ast->type == AND)
		ft_and(ast);
	else if (ast->type == OR)
		ft_or(ast);
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
