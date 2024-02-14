/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 03:37:36 by frapp             #+#    #+#             */
/*   Updated: 2024/02/14 04:05:12 by frapp            ###   ########.fr       */
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

int	redir_fd_write(char *file, bool append)
{
	int	fd;
	int	flag;

	flag = O_WRONLY | O_CREAT;
	if (append)
		flag |= O_APPEND;
	else
		flag |= O_TRUNC;
	fd = open(file, flag, NEW_FILE_PERMISSIONS);
	if (fd >= 0)
		return (fd);
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
	else
		print_error(true, "DEBUG redir_fd_write", file, strerror(errno));
	return (fd);
}

int	redir_read(char *file)
{
	int	fd;
	int	flag;

	flag = O_RDONLY;
	fd = open(file, flag);
	if (fd >= 0)
		return (fd);
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
	return (fd);
}

bool	check_valid_arg(t_ast *ast, t_redir *redir)
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
		return (false);
	}
	return (true);
}

bool	reset_stdio(t_ast *ast)
{
	int	*fds;

	fds = ast->fd;
	fds[READ] = dup2(ast->base_fd[READ], STDIN_FILENO);
	if (fds[READ] < 0)
	{
		//print_error(true, NULL, NULL, "error redirecting input");
		ast->exit_status = 1;
		
		return (false);
	}
	fds[WRITE] = dup2(ast->base_fd[WRITE], STDOUT_FILENO);
	if (fds[WRITE] < 0)
	{
		//print_error(true, NULL, NULL, "error redirecting output");
		ast->exit_status = 1;
		return (false);
	}
	return (true);
}

bool	redir_stdio(t_ast *ast)
{
	int	*fds;

	fds = ast->fd;
	//if (fds[READ] != ast->base_fd[READ])
	{
		fds[READ] = dup2(fds[READ], STDIN_FILENO);
		if (fds[READ] < 0)
		{
			//perror(strerror(errno));
			//print_error(true, NULL, NULL, "error redirecting input");
			ast->exit_status = 1;
			return (false);
		}
	}
	//if (fds[WRITE] != ast->base_fd[WRITE])
	{
		fds[WRITE] = dup2(fds[WRITE], STDOUT_FILENO);
		if (fds[WRITE] < 0)
		{
			//perror(strerror(errno));
			//print_error(true, NULL, NULL, "error redirecting ouput");
			ast->exit_status = 1;
			return (false);
		}
	}
	return (true);
}

t_result	resolve_redirs(t_ast *ast)
{
	t_redir	*redir;
	int		*fd;

	fd = ast->fd;
	redir = ast->redir;
	while (redir)
	{
		if (!check_valid_arg(ast, redir))
			return (ERROR);
		if (redir->type == REDIR_OUT)
		{
			if (fd[WRITE] != 1)
				close(fd[WRITE]);
			fd[WRITE] = redir_fd_write(redir->arg->name->token->str_data, false);
		}
		else if (redir->type == REDIR_APPEND)
		{
			if (fd[WRITE] != 1)
				close(fd[WRITE]);
			fd[WRITE] = redir_fd_write(redir->arg->name->token->str_data, true);
		}
		else if (redir->type == REDIR_IN)
		{
			if (fd[READ] != 0)
				close(fd[READ]);
			fd[READ] = redir_read(redir->arg->name->token->str_data);
		}
		// TODO
		else if (redir->type == HERE_DOC)
		{
			if (fd[READ] != 0)
				close(fd[READ]);
			fd[READ] = 0;
		}
		if (fd[WRITE] < 0 || fd[READ] < 0)
			return (ERROR);
		redir = redir->next;
	}
	return (SUCCESS);
}
