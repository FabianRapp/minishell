/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd1.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 07:42:31 by frapp             #+#    #+#             */
/*   Updated: 2024/02/25 07:34:22 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

t_fd_pair	*io_data(int flag, void *data)
{
	static t_fd_pair	*fds = NULL;

	if (flag == SET_NEW_FDS)
	{
		if (fds)
			reset_fds();
		fds = (t_fd_pair *)data;
	}
	else if (flag == RESET_FDS)
	{
		//fds = NULL;
	}
	else if (flag == CLEANUP_FDS)
		fds = NULL;
	return (fds);
}

// witout pipes mixed with redirs cause bugs
t_result	reset_stdio(int flag)
{
	static int	std_write = INIT_VAL;
	static int	std_read = INIT_VAL;

	if (flag == RESET_STDIO_CLEAN)
	{
		if (std_write != INIT_VAL)
			close(std_write);
		std_write = INIT_VAL;
		if (std_read != INIT_VAL)
			close(std_read);
		std_read = INIT_VAL;
		return (errno_to_result());
	}
	if (flag == RESET_STDIO_INIT)
	{
		reset_stdio(RESET_STDIO_CLEAN);
		std_read = dup(READ);
		std_write = dup(WRITE);
		return (errno_to_result());
	}
	//for fd leak function:
	if (flag == RESET_STDIO_GET_VALS)
	{
		int	return_val = 0;
		if (std_write != INIT_VAL)
			return_val = std_write * 1000;
		if (std_read != INIT_VAL)
			return_val += std_read;
		return (return_val);
	}
	if (std_read == INIT_VAL || std_write == INIT_VAL)
		return (ERROR);
	dup2(std_write, WRITE);
	dup2(std_read, READ);
	return (errno_to_result());
}

t_result	redir_fds(void)
{
	t_fd_pair	*fds;
	int			i;

	fds = get_fds();
	i = 0;
	while (fds && fds[i].base_fd != INIT_VAL)
	{
		dup2(fds[i].overload_with_fd, fds[i].base_fd);
		if (errno)
			return (print_error(true, NULL, NULL, strerror(errno)), ERROR);
		i++;
	}
	return (SUCCESS);
}

t_result	reset_fds(void)
{
	t_fd_pair	*fds;
	int			i;

	fds = get_fds();
	i = 0;
	while (fds && fds[i].base_fd != INIT_VAL)
	{
		//close(fds[i].overload_with_fd);
		dup2(fds[i].base_fd_backup, fds[i].base_fd);
		i++;
	}
	reset_stdio(RESET_STDIO);
	if (errno)
	{
		print_error(true, NULL, NULL, strerror(errno));
		return (ERROR);
	}
	io_data(RESET_FDS, NULL);
	return (SUCCESS);
}

t_result	cleanup_fds(void)
{
	t_fd_pair	*fds;
	int			i;

	if (reset_fds() == ERROR)
	{
		printf("rest_fds error\n");
		exit(1);
	}
	fds = get_fds();
	i = 0;
	while (fds && fds[i].base_fd != INIT_VAL)
	{
		close(fds[i].base_fd_backup);
		close(fds[i++].overload_with_fd);
	}
	io_data(CLEANUP_FDS, NULL);
	free(fds);
	return (SUCCESS);
}

t_fd_pair	*get_fds(void)
{
	return (io_data(GET_FDS, NULL));
}
