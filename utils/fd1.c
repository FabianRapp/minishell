/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd1.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 07:42:31 by frapp             #+#    #+#             */
/*   Updated: 2024/03/17 19:25:37 by frapp            ###   ########.fr       */
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

// Redirects file descriptors based on the prepared fd pairs before command execution.
// Iterates over fd pairs, using dup2 to apply redirections for command input/output.
// Ensures commands use the correct inputs and outputs, including here-doc content.
t_result	redir_fds(void)
{
	t_fd_set	*fds;
	int			i;

	// printf("BEFORE redir_fds():\n");
	// check_fds();
	// print_fds();
	fds = get_fds();
	i = 0;
	while(fds + i && !is_buffer_all_zeros(fds + i, sizeof(t_fd_set)))
	{
		dup2(fds[i].overload_with_fd, fds[i].base_fd);
		if (errno)
		{
			errno = 0;
			return (print_error(true, NULL, NULL, strerror(errno)), ERROR);
		}
		i++;
	}
	// printf("AFTER redir_fds():\n");
	// check_fds();
	//print_fds();
	return (SUCCESS);
}

// Resets file descriptors to their original state after command execution.
// Uses stored backup file descriptors to restore the original file descriptor state.
// Ensures the shell's file descriptor environment is clean for subsequent commands.
t_result	reset_fds(void)
{
	t_fd_set	*fds;
	int			i;

	fds = get_fds();
	i = 0;
	errno = 0;
	while(fds + i && !is_buffer_all_zeros(fds + i, sizeof(t_fd_set)))
	{
		//close(fds[i].overload_with_fd);
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

// Cleans up file descriptors and resets them, freeing associated resources.
// Closes both original and duplicated file descriptors, and frees the fd pairs array.
// Calls `reset_fds` to ensure a clean state before performing cleanup actions.
t_result	cleanup_fds(void)
{
	t_fd_set	*fds;
	int			i;
	t_result	return_val;

	return_val = reset_fds();
	fds = get_fds();
	i = 0;
	//while (fds && fds[i].base_fd != INIT_VAL)
	while(fds + i && !is_buffer_all_zeros(fds + i, sizeof(t_fd_set)))
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

//debugging from here on


#include <sys/syslimits.h>
#include <fcntl.h>


char	*get_file_name(int fd)
{
	char	*filePath = ft_calloc(1, PATH_MAX);
	char	*file_name;

	if (filePath == NULL)
		return NULL;
	int	old_errno = errno;
	if (fcntl(fd, F_GETPATH, filePath) != -1)
	{
		errno = old_errno;
		return (NULL);
	}
	file_name = extract_command_name(filePath);
	free(filePath);
	return (file_name);
}
void	print_fds(void)
{
	t_fd_set	*fds;
	char		*base_fd_str;
	char		*over_load_fd_str;
	char		*backup_fd_str;

	printf("\n");
	fds = io_data(-1, NULL);
	printf("| base_fd | overload_with_fd | backup_fd |\n");
	//while (fds && fds->overload_with_fd != INIT_VAL)
	while(fds && !is_buffer_all_zeros(fds, sizeof(t_fd_set)))
	{
		base_fd_str = get_file_name(fds->base_fd);
		over_load_fd_str = get_file_name(fds->overload_with_fd);
		backup_fd_str = get_file_name(fds->base_fd_backup);
		printf("| %d: %s | %d: %s | %d: %s |\n", fds->base_fd, base_fd_str, fds->overload_with_fd, over_load_fd_str, fds->base_fd_backup, backup_fd_str);
		free(base_fd_str);
		free(over_load_fd_str);
		free(backup_fd_str);
		fds++;
	}
	printf("\n");
}
