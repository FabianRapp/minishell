/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_redir_error_handler.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 04:24:43 by frapp             #+#    #+#             */
/*   Updated: 2024/03/07 04:27:40 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "repl_redir_internals.h"

t_result	handle_open_error(t_ast *ast, t_fd_pair *fd_pair)
{
	if (fd_pair->overload_with_fd != -1)
		return (SUCCESS);
	ast->exit_status = errno;
	set_last_exit(errno);
	print_error(true, NULL, NULL, strerror(errno));
	errno = 0;
	return (io_data(SET_NEW_FDS, NULL), ERROR);
}

// for add_fd_pair()
// is called if the left redir arg was an unopend fd
// this function basically makes the redir useless by making the dup2 args
// later on the same
// by doing this it is ensured that the execution is not disrupted as well as
// the cleanup cleans up correctly
t_result	handle_closed_fd_arg(t_fd_pair *fds, t_fd_pair *new_fd_pair)
{
	errno = 0;
	new_fd_pair->base_fd = new_fd_pair->overload_with_fd;
	fds->base_fd_backup = dup(new_fd_pair->base_fd);
	if (fds->base_fd_backup == -1)
		return (ERROR);
	return (SUCCESS);
}

void	*add_fd_pair_error(t_fd_pair *fds, t_fd_pair new_fd_pair)
{
	set_last_exit(errno);
	print_error(true, NULL, NULL, strerror(errno));
	errno = 0;
	if (new_fd_pair.overload_with_fd != INIT_VAL)
		close(new_fd_pair.overload_with_fd);
	if (new_fd_pair.base_fd_backup != INIT_VAL)
		close(new_fd_pair.base_fd_backup);
	io_data(SET_NEW_FDS, fds);
	cleanup_fds();
	return (NULL);
}

// TODO: first if not correct (this can be many diffrent errors)
t_result	check_valid_arg(t_ast *ast, t_redir *redir)
{
	if (count_args(redir->arg) != 1)
	{
		if (count_args(redir->arg) == 0)
		{
			print_error(true, false, false, type_to_str(T_EOF));
			ast->exit_status = 2;
			set_last_exit(2);
		}
		else
		{
			print_error(true, false, redir->arg->name->token->old_data,
				"ambiguous redirect");
			ast->exit_status = 1;
			set_last_exit(1);
		}
		return (ERROR);
	}
	return (SUCCESS);
}
