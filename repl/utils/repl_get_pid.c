/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_get_pid.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 08:53:04 by frapp             #+#    #+#             */
/*   Updated: 2024/03/06 08:54:58 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

// for child (new main process)
static int	catch_pid(int fd[2])
{
	int	pid;

	if (close(fd[WRITE]) == -1 || read(fd[READ], &pid, sizeof(pid_t)) == -1)
	{
		print_error(true, NULL, NULL, strerror(errno));
		close(fd[READ]);
		exit(errno);
	}
	if (close(fd[READ]) == -1)
	{
		print_error(true, NULL, NULL, strerror(errno));
		exit(errno);
	}
	return (pid);
}

// for parent
// always calls exit()
static void	send_pid(int fd[2], int pid)
{
	int	exit_status;

	if (close(fd[READ]) == -1 || write(fd[WRITE], &pid, sizeof(pid_t)) == -1)
	{
		close(fd[WRITE]);
		exit(errno);
	}
	if (close(fd[WRITE]) == -1)
		exit(errno);
	if (waitpid(pid, &exit_status, 0) == -1)
		exit(errno);
	exit_status = WEXITSTATUS(exit_status);
	exit(exit_status);
}

// since getpid() is forbidden this will launch a child process
// which becomes the main process
// the parent sends the new main processes pid via a pipe to it
int	get_pid(void)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
		return (print_error(true, false, false, "Error creating pipe\n"), 0);
	pid = fork();
	if (pid == -1)
	{
		print_error(true, NULL, NULL, strerror(errno));
		exit(errno);
	}
	errno = 0;
	if (pid > 0)
	{
		send_pid(fd, pid);
	}
	return (catch_pid(fd));
}
