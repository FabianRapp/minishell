/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd1.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 07:42:31 by frapp             #+#    #+#             */
/*   Updated: 2024/03/25 00:41:27 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

t_fd_set	*io_data(int flag, void *data)
{
	static t_fd_set	*fds = NULL;

	if (flag == SET_NEW_FDS)
	{
		if (fds)
			cleanup_fds();
		fds = (t_fd_set *)data;
	}
	else if (flag == CLEANUP_FDS)
		fds = NULL;
	return (fds);
}

// Redirects fds based on the prepared fd sets before command execution.
// uses dup2 to apply redirections for command input/output
// on each fd set.
// Ensures commands use the correct inputs and outputs,
// including here-doc and here-str content.
t_result	redir_fds(void)
{
	t_fd_set	*fds;
	int			i;

	fds = get_fds();
	i = 0;
	while (fds + i && !is_buffer_all_zeros(fds + i, sizeof(t_fd_set)))
	{
		dup2(fds[i].overload_with_fd, fds[i].base_fd);
		if (errno)
		{
			errno = 0;
			return (print_error(true, NULL, NULL, strerror(errno)), ERROR);
		}
		i++;
	}
	return (SUCCESS);
}

// Resets fds to their original state after command execution.
// Uses stored backup fds to restore the original file descriptor state.
// Ensures the shell's fd environment is clean for subsequent commands.
t_result	reset_fds(void)
{
	t_fd_set	*fds;
	int			i;

	fds = get_fds();
	i = 0;
	errno = 0;
	while (fds + i && !is_buffer_all_zeros(fds + i, sizeof(t_fd_set)))
	{
		dup2(fds[i].base_fd_backup, fds[i].base_fd);
		i++;
	}
	if (errno)
	{
		print_error(true, NULL, NULL, strerror(errno));
		full_exit_status(true);
		return (ERROR);
	}
	return (SUCCESS);
}

// Cleans up fds and resets them, freeing associated resources.
// Closes both original and duplicated fds, and frees the fd sets array.
// Calls `reset_fds` to ensure a clean state.
t_result	cleanup_fds(void)
{
	t_fd_set	*fds;
	int			i;
	t_result	return_val;

	return_val = reset_fds();
	fds = get_fds();
	i = 0;
	while (fds + i && !is_buffer_all_zeros(fds + i, sizeof(t_fd_set)))
	{
		close(fds[i].base_fd_backup);
		close(fds[i++].overload_with_fd);
	}
	io_data(CLEANUP_FDS, NULL);
	free(fds);
	return (return_val);
}

t_fd_set	*get_fds(void)
{
	return (io_data(GET_FDS, NULL));
}
