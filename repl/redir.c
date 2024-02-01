/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 03:37:36 by frapp             #+#    #+#             */
/*   Updated: 2024/02/01 11:57:32 by frapp            ###   ########.fr       */
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
     [EFAULT]           Path points outside the process's allocated address space.
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

int	redir_out(char *file, bool append)
{
	int	fd;
	int	flag;

	flag = O_WRONLY | O_CREAT;
	if (append)
		flag |= O_APPEND;
	fd = open(file, flag);
	if (fd >= 0)
		return (fd);
	if (fd == EACCES)
		print_error(true, NULL, file, "Permission denied");
	else if (fd == EINTR)
		print_error(true, NULL, file, "Signal interupt");
	else if (fd == EIO)
		print_error(true, NULL, file, "Error writing to file"); //change for in
	else if (fd == EISDIR)
		print_error(true, NULL, file, "Is a directory");// write only
	else if (fd == EMFILE)
		print_error(true, NULL, file, "Too many open files");
	else if (fd == ENAMETOOLONG)
		print_error(true, NULL, file, "File name too long");
	else if (fd == ENOENT)
		print_error(true, NULL, file, "No such file or directory");
	else if (fd == ENOTDIR)
		print_error(true, NULL, file, "Not a directory");
	else
		print_error(true, NULL, file, "Error");
	return (fd);
}


bool	resolve_redirs(t_ast *ast)
{
	t_arg	*redirs;

	redirs = ast->redir_in;
	while (redirs)
	{
		if (redirs->name->next)
		{
			// TODO: change additional redir args to be in name rather new args and throw
			// and error here if multiple args for 1 redir
		}
		redirs = redirs->next;
	}
	if (ast->redir_out)
	{
		
	}
	return (true);
}
