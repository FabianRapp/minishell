/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 16:02:33 by frapp             #+#    #+#             */
/*   Updated: 2024/03/21 16:02:47 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_result	pipe_error_handler(t_pipe_data *vars)
{
	set_errno_as_exit(vars->ast, true);
	if (vars->base_write != INIT_VAL)
	{
		dup2(vars->base_write, WRITE);
		ft_close(&(vars->base_write));
	}
	if (vars->base_read != INIT_VAL)
	{
		dup2(vars->base_read, READ);
		if (vars->ast->fd_to_close_read == INIT_VAL)
			ft_close(&(vars->base_read));
	}
	if (vars->pipe_fd[READ] != INIT_VAL)
		ft_close(&(vars->pipe_fd[READ]));
	if (vars->pipe_fd[WRITE] != INIT_VAL)
		ft_close(&(vars->pipe_fd[WRITE]));
	return (ERROR);
}
