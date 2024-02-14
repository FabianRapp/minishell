/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 03:37:36 by frapp             #+#    #+#             */
/*   Updated: 2024/02/14 13:53:32 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

/*
     []           Search permission is denied for a component of the path prefix.
     []           The required permissions (for reading and/or writing) are denied for the given flags.
     []           O_CREAT is specified, the file does not exist, and the directory in which it is to be created does not permit writing.
     []           O_TRUNC is specified and write permission is denied.
     []           path specifies the slave side of a locked pseudo-terminal device.
     []           O_CREAT is specified, the file does not exist, and the directory in which the entry for the new file is being placed cannot be extended because the user's quota of disk blocks on the file system containing the directory has been
                        exhausted.
     []           O_CREAT is specified, the file does not exist, and the user's quota of inodes on the file system on which the file is being created has been exhausted.
     [EEXIST]           O_CREAT and O_EXCL are specified and the file exists.
     [EFAULT]           Path points fd[fd[out]]side the process's allocated address space.
     []            The open() operation is interrupted by a signal.
     [EINVAL]           The value of oflag is not valid.
     []              An I/O error occurs while making the directory entry or allocating the inode for O_CREAT.
     []           The named file is a directory, and the arguments specify that it is to be opened for writing.
     [ELOOP]            Too many symbolic links are encountered in translating the pathname.  This is taken to be indicative of a looping symbolic link.
     []           The process has already reached its limit for open file descriptors.
     []     A component of a pathname exceeds {NAME_MAX} characters, or an entire path name exceeded {PATH_MAX} characters.
     [ENFILE]           The system file table is full.
     [ELOOP]            O_NOFOLLOW was specified and the target is a symbolic link.
     []           O_CREAT is not set and the named file does not exist.
     []           A component of the path name that must exist does not exist.
     [ENOSPC]           O_CREAT is specified, the file does not exist, and the directory in which the entry for the new file is being placed cannot be extended because there is no space left on the file system containing the directory.
     [ENOSPC]           O_CREAT is specified, the file does not exist, and there are no free inodes on the file system on which the file is being created.
     []          A component of the path prefix is not a directory.
     [ENXIO]            The named file is a character-special or block-special file and the device associated with this special file does not exist.
     [ENXIO]            O_NONBLOCK and O_WRONLY are set, the file is a FIFO, and no process has it open for reading.
     [EOPNOTSUPP]       O_SHLOCK or O_EXLOCK is specified, but the underlying filesystem does not support locking.
     [EOPNOTSUPP]       An attempt is made to open a socket (not currently implemented).
     [EOVERFLOW]        The named file is a regular file and its size does not fit in an object of type off_t.
     [EROFS]            The named file resides on a read-only file system, and the file is to be modified.
     [ETXTBSY]          The file is a pure procedure (shared text) file that is being executed and the open() call requests write access.
     [EBADF]            The path argument does not specify an absolute path and the fd argument is neither AT_FDCWD nor a valid file descriptor open for searching.
     [ENOTDIR]          The path argument is not an absolute path and fd is neither AT_FDCWD nor a file descriptor associated with a directory.
     [EILSEQ]           The filename does not match the encoding rules.
*/

t_fd_pair	*io_data(int flag, void *data)
{
	static t_fd_pair	*fds = NULL;

	if (flag == SET_NEW_FDS)
	{
		if (fds)
			reset_fds();
		fds = (t_fd_pair *)data;
	}
	if (flag == RESET_FDS)
		fds = NULL;
	return (fds);
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
		close(fds[i].overload_with_fd);
		dup2(fds[i].base_fd_backup, fds[i].base_fd);
		i++;
	}
	if (errno)
	{
		print_error(true, NULL, NULL, strerror(errno));
		return (ERROR);
	}
	free(fds);
	io_data(RESET_FDS, NULL);
	return (SUCCESS);
}

void	set_new_fds(t_fd_pair *fds)
{
	io_data(SET_NEW_FDS, fds);
}

t_fd_pair	*get_fds(void)
{
	return (io_data(GET_FDS, NULL));
}


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
	else if (errno == ENAMETOOLONG)
		print_error(true, "DEBUG redir_read", file, "File name too long");
	else if (errno == ENOENT)
		print_error(true, "DEBUG redir_read", file, "No such file or directory");
	else if (errno == ENOTDIR)
		print_error(true, "DEBUG redir_read", file, "Not a directory");
	else
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

t_result	find_and_replace_from(t_fd_pair *fds, int base_fd, int overload_with_fd)
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

	if (find_and_replace_from(fds, new_fd_pair.base_fd, new_fd_pair.overload_with_fd) == SUCCESS)
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
	int			*fd;
	int			base_fd;
	t_fd_pair	new_fd_pair;
	t_fd_pair	*fds;

	fd = ast->fd;
	redir = ast->redir;
	fds = NULL;
	new_fd_pair.overload_with_fd = ast->fd[WRITE];
	new_fd_pair.base_fd = WRITE;
	fds = add_fd_pair(fds, new_fd_pair);
	if (!fds)
		return (ERROR);
	new_fd_pair.overload_with_fd = ast->fd[READ];
	new_fd_pair.base_fd = READ;
	fds = add_fd_pair(fds, new_fd_pair);
	if (!fds)
		return (ERROR);
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
		if (fd[WRITE] < 0 || fd[READ] < 0)
			return (ERROR);
		redir = redir->next;
	}
	set_new_fds(fds);
	return (SUCCESS);
}
