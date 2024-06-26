/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 03:37:36 by frapp             #+#    #+#             */
/*   Updated: 2024/03/26 22:27:06 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_fd_set	redir_fd_write(char *file, bool append, int base_fd)
{
	int			flag;
	t_fd_set	fd_pair;

	fd_pair.base_fd = WRITE;
	if (base_fd != INIT_VAL)
	{
		fd_pair.base_fd = base_fd;
	}
	flag = O_WRONLY | O_CREAT;
	if (append)
		flag |= O_APPEND;
	else
		flag |= O_TRUNC;
	fd_pair.overload_with_fd = open(file, flag, NEW_FILE_PERMISSIONS);
	return (fd_pair);
}

t_fd_set	redir_read(char *file, int base_fd, bool in_out)
{
	int			flag;
	t_fd_set	fd_pair;

	fd_pair.base_fd = READ;
	if (base_fd != INIT_VAL)
	{
		fd_pair.base_fd = base_fd;
	}
	if (in_out)
		flag = O_RDWR;
	else
		flag = O_RDONLY;
	fd_pair.overload_with_fd = open(file, flag);
	return (fd_pair);
}

static t_result	find_and_replace_existing(t_fd_set *fds,
	int base_fd, int overload_with_fd)
{
	int	i;

	i = 0;
	while (fds && !is_buffer_all_zeros(fds + i, sizeof(t_fd_set)))
	{
		if (fds[i].base_fd == base_fd)
		{
			if (close(fds[i].overload_with_fd) == -1)
				return (ERROR);
			fds[i].overload_with_fd = overload_with_fd;
			return (SUCCESS);
		}
		i++;
	}
	return (ERROR);
}

// returns new len, -1 on error
int	extend_fd_array(t_fd_set **fds)
{
	int			len;
	t_fd_set	*new;

	len = 0;
	while (*fds && !is_buffer_all_zeros((*fds)
			+ len, sizeof(t_fd_set)))
	{
		len++;
	}
	new = ft_calloc(len + 2, sizeof(t_fd_set));
	if (!new)
		return (-1);
	ft_memcpy(new, *fds, sizeof(t_fd_set) * len);
	free(*fds);
	*fds = new;
	return (len);
}

t_fd_set	*add_fd_pair(t_fd_set *fds, t_fd_set new_fd_pair)
{
	int	old_len;

	errno = 0;
	if (find_and_replace_existing(fds, new_fd_pair.base_fd,
			new_fd_pair.overload_with_fd) == SUCCESS)
		return (fds);
	if (errno)
		return (add_fd_pair_error(fds, new_fd_pair));
	old_len = extend_fd_array(&fds);
	if (old_len == -1)
	{
		return (add_fd_pair_error(fds, new_fd_pair));
	}
	fds[old_len].base_fd_backup = dup(new_fd_pair.base_fd);
	if ((errno && errno != 9) || (errno == 9
			&& handle_closed_fd_arg(fds + old_len, &new_fd_pair) == ERROR))
	{
		return (add_fd_pair_error(fds, new_fd_pair));
	}
	fds[old_len].base_fd = new_fd_pair.base_fd;
	fds[old_len].overload_with_fd = new_fd_pair.overload_with_fd;
	return (fds);
}
