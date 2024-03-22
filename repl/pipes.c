/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 03:48:26 by frapp             #+#    #+#             */
/*   Updated: 2024/03/21 16:04:38 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

t_result	pipe_init_left(t_pipe_data *vars)
{
	vars->base_write = dup(WRITE);
	if (vars->base_write == -1)
	{
		pipe_error_handler(vars);
		return (ERROR);
	}
	if (pipe(vars->pipe_fd) == -1)
	{
		pipe_error_handler(vars);
		return (ERROR);
	}
	if (dup2(vars->pipe_fd[WRITE], WRITE) == -1
		|| ft_close(&(vars->pipe_fd[WRITE])) == -1)
	{
		pipe_error_handler(vars);
		return (ERROR);
	}
	return (SUCCESS);
}

t_result	pipe_init_right(t_pipe_data *vars)
{
	if (dup2(vars->base_write, WRITE) == -1
		|| ft_close(&(vars->base_write)) == -1)
	{
		pipe_error_handler(vars);
		return (ERROR);
	}
	vars->ast->shared_data->stop_execution = false;
	vars->base_read = vars->ast->fd_to_close_read;
	if (vars->base_read == INIT_VAL)
		vars->base_read = dup(READ);
	if (vars->base_read == -1)
	{
		pipe_error_handler(vars);
		return (ERROR);
	}
	if (dup2(vars->pipe_fd[READ], READ) == -1
		|| ft_close(&(vars->pipe_fd[READ])) == -1)
	{
		pipe_error_handler(vars);
		return (ERROR);
	}
	vars->ast->right->fd_to_close_read = vars->base_read;
	return (SUCCESS);
}

static t_result	pipe_fork_left(t_pipe_data *vars)
{
	vars->left_pid = fork();
	if (vars->left_pid < 0)
		return (pipe_error_handler(vars));
	else if (vars->left_pid == 0)
	{
		errno = 0;
		if (vars->ast->fd_to_close_read == INIT_VAL)
			ft_close(&(vars->base_read));
		ft_close(&(vars->pipe_fd[READ]));
		run_node(vars->ast->left);
		if (vars->ast->left->exit_status == DEFAULT_EXIT_STATUS)
		{
			waitpid(vars->ast->left->pid, &(vars->ast->left->exit_status), 0);
			vars->ast->left->exit_status
				= WEXITSTATUS(vars->ast->left->exit_status);
		}
		exit(vars->ast->left->exit_status);
	}
	return (SUCCESS);
}

static t_result	pipe_fork_right(t_pipe_data *vars)
{
	if (vars->ast->right->type != COMMAND && vars->ast->right->type != SUBSHELL)
	{
		run_node(vars->ast->right);
		vars->ast->pid = vars->ast->right->pid;
		vars->ast->exit_status = vars->ast->right->exit_status;
		return (SUCCESS);
	}
	vars->ast->pid = fork();
	if (vars->ast->pid < 0)
		return (pipe_error_handler(vars));
	else if (vars->ast->pid == 0)
	{
		errno = 0;
		if (vars->ast->fd_to_close_read == INIT_VAL)
			ft_close(&(vars->base_read));
		run_node(vars->ast->right);
		if (vars->ast->right->exit_status == DEFAULT_EXIT_STATUS)
		{
			waitpid(vars->ast->right->pid, &(vars->ast->right->exit_status), 0);
			vars->ast->right->exit_status
				= WEXITSTATUS(vars->ast->right->exit_status);
		}
		exit(vars->ast->right->exit_status);
	}
	return (SUCCESS);
}

t_result	ft_pipe(t_ast *ast)
{
	t_pipe_data					vars;
	static const t_pipe_data	init_val = {{-1, -1}, -1, -1, -1, NULL};

	if (ast->shared_data->stop_execution)
	{
		ast->exit_status = 1;
		return (SUCCESS);
	}
	vars = init_val;
	vars.ast = ast;
	if (pipe_init_left(&vars) == ERROR)
		return (ERROR);
	if (pipe_fork_left(&vars) == ERROR)
		return (ERROR);
	if (pipe_init_right(&vars) == ERROR)
		return (ERROR);
	if (pipe_fork_right(&vars) == ERROR)
		return (ERROR);
	(dup2(vars.base_read, READ) == -1 && pipe_error_handler(&vars));
	return ((ast->fd_to_close_read == INIT_VAL
			&& ft_close(&(vars.base_read))), SUCCESS);
}
