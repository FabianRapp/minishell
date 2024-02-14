/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 07:42:31 by frapp             #+#    #+#             */
/*   Updated: 2024/02/14 13:04:14 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

// this file gets likely completly deleted

int	base_fd(struct fd_request request_data)
{
	static int	base_write = WRITE;
	static int	base_read = READ;
	static int	base_error = 2;

	if (request_data.get_read != FD_REQUEST_SKIP)
		return (base_read);
	if (request_data.get_write != FD_REQUEST_SKIP)
		return (base_write);
	if (request_data.get_error != FD_REQUEST_SKIP)
		return (base_error);
	if (request_data.set_read != FD_REQUEST_SKIP)
		base_read = request_data.set_read;
	if (request_data.set_write != FD_REQUEST_SKIP)
		base_write = request_data.set_write;
	if (request_data.set_error != FD_REQUEST_SKIP)
		base_error = request_data.set_error;
	return (FD_REQUEST_SKIP);
}

static struct fd_request	new_request_data(void)
{
	struct fd_request	request_data;

	request_data.get_error = INIT_VAL;
	request_data.get_read = INIT_VAL;
	request_data.get_write = INIT_VAL;
	request_data.set_error = INIT_VAL;
	request_data.set_read = INIT_VAL;
	request_data.set_write = INIT_VAL;
	return (request_data);
}

void	init_ioe(void)
{
	struct fd_request	request_data;

	request_data = new_request_data();
	request_data.set_write = WRITE;
	request_data.set_read = READ;
	request_data.set_error = 2;
	base_fd(request_data);
}

int	get_base_write(void)
{
	struct fd_request	request_data;

	request_data = new_request_data();
	request_data.get_write = 1;
	return (base_fd(request_data));
}

int	get_base_read(void)
{
	struct fd_request	request_data;

	request_data = new_request_data();
	request_data.get_read = 1;
	return (base_fd(request_data));
}

int	get_base_error(void)
{
	struct fd_request	request_data;

	request_data = new_request_data();
	request_data.get_error = 1;
	return (base_fd(request_data));
}

void	set_base_write(int fd)
{
	struct fd_request	request_data;

	request_data = new_request_data();
	request_data.set_write = fd;
	base_fd(request_data);
}

void	set_base_read(int fd)
{
	struct fd_request	request_data;

	request_data = new_request_data();
	request_data.set_read = fd;
	base_fd(request_data);
}

void	set_base_error(int fd)
{
	struct fd_request	request_data;

	request_data = new_request_data();
	request_data.set_error = fd;
	base_fd(request_data);
}
