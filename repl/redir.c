/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 03:37:36 by frapp             #+#    #+#             */
/*   Updated: 2024/02/15 05:22:20 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

t_fd_pair	redir_fd_write(char *file, bool append, int base_fd)
{
	int			flag;
	t_fd_pair	fd_pair;

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
	if (errno == EACCES)
		print_error(true, "DEBUG redir_fd_write", file, "Permission denied");
	else if (errno == EINTR)
		print_error(true, "DEBUG redir_fd_write", file, "Signal interupt");
	else if (errno == EIO)
		print_error(true, "DEBUG redir_fd_write", file, "Error writing to file");
	else if (errno == EISDIR)
		print_error(true, "DEBUG redir_fd_write", file, "Is a directory");
	else if (errno == EMFILE)
		print_error(true, "DEBUG redir_fd_write", file, "Too many open files");
	else if (errno == ENAMETOOLONG)
		print_error(true, "DEBUG redir_fd_write", file, "File name too long");
	else if (errno == ENOENT)
		print_error(true, "DEBUG redir_fd_write", file, "No such file or directory");
	else if (errno == ENOTDIR)
		print_error(true, "DEBUG redir_fd_write", file, "Not a directory");
	else if (errno)
		print_error(true, "DEBUG redir_fd_write else", file, strerror(errno));
	errno = 0;
	return (fd_pair);
}

t_fd_pair	redir_read(char *file, int base_fd)
{
	int			flag;
	t_fd_pair	fd_pair;

	fd_pair.base_fd = READ;
	if (base_fd != INIT_VAL)
	{
		fd_pair.base_fd = base_fd;
	}
	flag = O_RDONLY;
	fd_pair.overload_with_fd = open(file, flag);
	if (errno == EACCES)
		print_error(true, "DEBUG redir_read", file, "Permission denied");
	else if (errno == EINTR)
		print_error(true, "DEBUG redir_read", file, "Signal interupt");
	else if (errno == EIO)
		print_error(true, "DEBUG redir_read", file, "Error reading from file"); 
	else if (errno == EISDIR)
		print_error(true, "DEBUG redir_read", file, "Is a directory");
	else if (errno == EMFILE)
		print_error(true, "DEBUG redir_read", file, "Too many open files");
	else if (errno == ENOENT)
		print_error(true, "DEBUG redir_read", file, "No such file or directory");
	else if (errno == ENOTDIR)
		print_error(true, "DEBUG redir_read", file, "Not a directory");
	else if (errno)
		print_error(true, "DEBUG redir_read", file, strerror(errno));
	errno = 0;
	return (fd_pair);
}

t_result	check_valid_arg(t_ast *ast, t_redir *redir)
{
	if (count_args(redir->arg) != 1)
	{
		if (count_args(redir->arg) == 0)
			print_error(true, false, false, type_to_str(T_EOF));
		else
		{
			print_error(true, false, redir->arg->name->token->old_data, "ambiguous redirect");
			ast->exit_status = 1;
		}
		return (ERROR);
	}
	return (SUCCESS);
}

t_result	find_and_replace_existing(t_fd_pair *fds, int base_fd, int overload_with_fd)
{
	int	i;

	i = 0;
	while (fds && fds[i].base_fd != INIT_VAL)
	{
		if (fds[i].base_fd == base_fd)
		{
			close(fds[i].overload_with_fd);
			fds[i].overload_with_fd = overload_with_fd;
			return (SUCCESS);
		}
		i++;
	}
	return (ERROR);
}

// returns new len
int	extend_fd_array(t_fd_pair **fds)
{
	int			len;
	t_fd_pair	*new;

	len = 0;
	while (*fds && (*fds)[len].base_fd != INIT_VAL)
	{
		len++;
	}
	new = ft_calloc(len + 2, sizeof(t_fd_pair));
	if (!new)
		return (free(*fds), -1);
	ft_memcpy(new, *fds, sizeof(t_fd_pair) * len);
	free(*fds);
	*fds = new;
	return (len);
}

t_fd_pair *add_fd_pair(t_fd_pair *fds, t_fd_pair new_fd_pair)
{
	int	len;

	if (find_and_replace_existing(fds, new_fd_pair.base_fd, new_fd_pair.overload_with_fd) == SUCCESS)
		return (fds);
	len = extend_fd_array(&fds);
	if (len == -1)
		return (NULL);
	errno = 0;
	fds[len].base_fd_backup = dup(fds[len].base_fd);
	if (errno)
	{//TODO add error handeling
		print_error(true, NULL, NULL, strerror(errno));
		exit(errno);
	}
	fds[len].base_fd = new_fd_pair.base_fd;
	fds[len].overload_with_fd = new_fd_pair.overload_with_fd;
	fds[len + 1].base_fd = INIT_VAL;
	fds[len + 1].base_fd_backup = INIT_VAL;
	fds[len + 1].overload_with_fd = INIT_VAL;
	return (fds);
}

t_result	resolve_redirs(t_ast *ast)
{
	t_redir		*redir;
	int			base_fd;
	t_fd_pair	new_fd_pair;
	t_fd_pair	*fds;

	redir = ast->redir;
	fds = NULL;
	// new_fd_pair.overload_with_fd = ast->pipe[WRITE];
	// new_fd_pair.base_fd = WRITE;
	// fds = add_fd_pair(fds, new_fd_pair);
	// if (!fds)
	// 	return (ERROR);
	// new_fd_pair.overload_with_fd = ast->pipe[READ];
	// new_fd_pair.base_fd = READ;
	// fds = add_fd_pair(fds, new_fd_pair);
	// if (!fds)
	// 	return (ERROR);
	
	while (redir)
	{
		if (!check_valid_arg(ast, redir))
			return (ERROR);
		base_fd = redir->left_redir_arg;
		if (redir->type == REDIR_OUT)
		{
			new_fd_pair = redir_fd_write(redir->arg->name->token->str_data, false, base_fd);
			fds = add_fd_pair(fds, new_fd_pair);
			if (!fds)
				return (ERROR);
		}
		else if (redir->type == REDIR_APPEND)
		{
			new_fd_pair = redir_fd_write(redir->arg->name->token->str_data, true, base_fd);
			if (new_fd_pair.overload_with_fd == -1)
			{// TODO error
				return (ERROR);
			}
			fds = add_fd_pair(fds, new_fd_pair);
			if (!fds)
				return (ERROR);
		}
		else if (redir->type == REDIR_IN)
		{
			new_fd_pair = redir_read(redir->arg->name->token->str_data, base_fd);
			fds = add_fd_pair(fds, new_fd_pair);
			if (!fds)
				return (ERROR);
		}
		// TODO
		else if (redir->type == HERE_DOC)
		{
		}
		redir = redir->next;
	}
	io_data(SET_NEW_FDS, fds);
	return (SUCCESS);
}
