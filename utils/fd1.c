/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd1.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 07:42:31 by frapp             #+#    #+#             */
/*   Updated: 2024/02/19 13:31:14 by frapp            ###   ########.fr       */
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
t_result	reset_stdio(void)
{
	static int	std_write = INIT_VAL;
	static int	std_read = INIT_VAL;

	if (std_write == INIT_VAL)
	{
		std_read = dup(READ);
		std_write = dup(WRITE);
	}
	else
	{
		dup2(std_write, WRITE);
		dup2(std_read, READ);
	}
	if (errno)
		return (ERROR);
	return (SUCCESS);
}

// always call this twice
// use this when unsure which state is currently active
t_result	temp_redir(void)
{
	static bool	first_run = true;
	static bool	reset = false;

	if (first_run)
	{
		first_run = false;
		if (!get_fds())
		{
			reset = true;
			return (redir_fds());
		}
		else
			reset = false;
	}
	else
	{
		first_run = true;
		if (reset)
			return (reset_fds());
	}
	return (SUCCESS);
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
	reset_stdio();
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
	return (SUCCESS);
}

t_fd_pair	*get_fds(void)
{
	return (io_data(GET_FDS, NULL));
}
