/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 03:48:26 by frapp             #+#    #+#             */
/*   Updated: 2024/03/19 05:04:30 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"


static t_result	fork_left(t_ast *ast, int base, int pipe_fd[2])
{
	int	pid;

	pid = fork();
	if (pid < 0)
	{
		set_errno_as_exit(ast, true);
		dup2(base, WRITE);
		close (base);
		close (pipe_fd[READ]);
		errno = 0;
		return (ERROR);
	}
	else if (pid == 0)
	{
		errno = 0;
		close (base);
		close (pipe_fd[READ]);
		run_node(ast->left);
		if (ast->left->exit_status == DEFAULT_EXIT_STATUS)
		{
			waitpid(ast->left->pid, &(ast->left->exit_status), 0);
			ast->left->exit_status = WEXITSTATUS(ast->left->exit_status);
		}
		exit(ast->left->exit_status);
	}
	return (SUCCESS);
}

static t_result	fork_right(t_ast *ast, int base)
{
	ast->pid = fork();
	if (ast->pid < 0)
	{
		set_errno_as_exit(ast, true);
		dup2(base, READ);
		//if (ast->fd_to_close_read == INIT_VAL)
			close(base);
		errno = 0;
		return (ERROR);
	}
	else if (ast->pid == 0)
	{
		errno = 0;
		close (base);
		run_node(ast->right);
		if (ast->right->exit_status == DEFAULT_EXIT_STATUS)
		{
			waitpid(ast->right->pid, &(ast->right->exit_status), 0);
			ast->right->exit_status = WEXITSTATUS(ast->right->exit_status);
		}
		exit(ast->right->exit_status);
	}
	return (SUCCESS);
}

void	ft_pipe(t_ast *ast)
{
	int			pipe_fd[2];
	int			base;

	if (ast->shared_data->stop_execution)
	{
		ast->exit_status = 1;
		return ;
	}
	//check_fds();
	base = dup(WRITE);
	//ast->left->fd_to_close_write = base;
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
	//ast->left->fd_to_close = pipe_fd[READ];
	if (fork_left(ast, base, pipe_fd) == ERROR)
		return ;
	if (dup2(base, WRITE) == -1)
	{
		close (base);
		close (pipe_fd[READ]);
		set_errno_as_exit(ast, true);
		return ;
	}
	close(base);
	ast->shared_data->stop_execution = false;
	//if (ast->fd_to_close_read != INIT_VAL)
		//base = ast->fd_to_close_read;
	//else
		base = dup(READ);
	if (base == -1)
	{
		close(pipe_fd[READ]);
		set_errno_as_exit(ast, true);
		return ;
	}
	if (dup2(pipe_fd[READ], READ) == -1)
	{
		//if (ast->fd_to_close_read == INIT_VAL)
			close(base);
		close(pipe_fd[READ]);
		set_errno_as_exit(ast, true);
		return ;
	}
	close(pipe_fd[READ]);
	ast->right->fd_to_close_read = base;
	if (fork_right(ast, base) == ERROR)
		return ;
	if (dup2(base, READ) == -1)
	{
		//if (ast->fd_to_close_read == INIT_VAL)
			close(base);
		set_errno_as_exit(ast, true);
		return ;
	}
	//if (ast->fd_to_close_read == INIT_VAL)
		close(base);
	check_fds();
}
