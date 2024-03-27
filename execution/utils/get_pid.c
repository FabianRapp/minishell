/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_pid.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 08:53:04 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 21:57:32 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

// for child (new main process)
static int	catch_pid(int fd[2])
{
	int	pid;

	if (close(fd[WRITE]) == -1 || read(fd[READ], &pid, sizeof(pid_t)) == -1)
	{
		close(fd[READ]);
		print_error(true, NULL, NULL, strerror(errno));
		exit(errno);
	}
	if (close(fd[READ]) == -1)
	{
		print_error(true, NULL, NULL, strerror(errno));
		exit(errno);
	}
	ft_pid(pid);
	return (pid);
}

// for parent
// always calls exit()
static void	send_pid(int fd[2], int pid)
{
	int				exit_status;
	struct termios	base_term;

	if (isatty(0) && tcgetattr(0, &base_term) == -1)
	{
		tcsetattr(0, TCSANOW, &base_term);
		exit(errno);
	}
	if (pid == -1 || close(fd[READ]) == -1 || write(fd[WRITE],
			&pid, sizeof(pid_t)) == -1 || close(fd[WRITE]) == -1)
	{
		if (isatty(0))
			tcsetattr(0, TCSANOW, &base_term);
		exit(errno);
	}
	if (waitpid(pid, &exit_status, 0) == -1)
	{
		tcsetattr(0, TCSANOW, &base_term);
		exit(errno);
	}
	exit_status = WEXITSTATUS(exit_status);
	if (isatty(0))
		tcsetattr(0, TCSANOW, &base_term);
	exit(exit_status);
}

// returns the pid
int	ft_pid(int set)
{
	static int	pid = -1;

	if (set)
		pid = set;
	return (pid);
}

// since getpid() is forbidden this will launch a child process
// which becomes the main process
// the parent sends the new main processes pid via a pipe to it
int	get_pid(void)
{
	pid_t				pid;
	int					fd[2];
	struct sigaction	sig;

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
		sigemptyset(&(sig.sa_mask));
		sig.sa_flags = 0;
		sig.sa_handler = SIG_IGN;
		if (sigaction(SIGQUIT, &sig, NULL) == -1
			|| sigaction(SIGINT, &sig, NULL) == -1)
			return (print_error(true, NULL, NULL, strerror(errno)),
				wait_all_children(NULL), exit(1), -1);
		send_pid(fd, pid);
	}
	return (catch_pid(fd));
}
