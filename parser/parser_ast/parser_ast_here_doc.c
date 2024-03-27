/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_here_doc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 04:56:10 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 07:49:50 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_result	here_doc_parent(char *termination, int pipe_fd[2], int pid)
{
	int		child_has_exited;
	int		child_exit_status;

	free(termination);
	close(pipe_fd[WRITE]);
	child_has_exited = 0;
	while (child_has_exited == 0 && !here_doc_exit_state(false, false))
	{
		child_has_exited = waitpid(pid, &child_exit_status, WNOHANG);
	}
	if (here_doc_exit_state(false, false))
	{
		return (close(pipe_fd[READ]), ERROR);
	}
	child_exit_status = WEXITSTATUS(child_exit_status);
	if (child_exit_status)
		return (set_last_exit(child_exit_status), close(pipe_fd[READ]), ERROR);
	return (SUCCESS);
}

t_result	fork_here_doc(char *termination, int pipe_fd[2], t_redir *redir)
{
	int		pid;

	here_doc_exit_state(true, false);
	pid = fork();
	if (pid == -1)
	{
		free(termination);
		close(pipe_fd[WRITE]);
		close(pipe_fd[READ]);
		set_last_exit(errno);
		print_error(true, NULL, NULL, strerror(errno));
		return (ERROR);
	}
	else if (pid > 0)
	{
		return (here_doc_parent(termination, pipe_fd, pid));
	}
	init_here_doc_child(pipe_fd, termination, redir);
	return (ERROR);
}

// Initializes a pipe and captures input until
//	a termination string is encountered.
// Stores the read-end file descriptor as a
//	string in the redir structure for later use.
// Writes each line of input to the write-end
//	of the pipe, excluding the termination line.
t_result	parser_resovle_here_doc(t_redir *redir)
{
	int				pipe_fd[2];
	char			*termination;
	char			*temp;

	termination = ft_strjoin(redir->token_str_data, "\n");
	if (!termination)
		return (ERROR);
	if (pipe(pipe_fd) == -1)
		return (set_last_exit(errno), free(termination), ERROR);
	free(redir->token_str_data);
	redir->token_str_data = ft_itoa(pipe_fd[READ]);
	if (!redir->token_str_data)
		return (close(pipe_fd[READ]), close(pipe_fd[WRITE]),
			free(termination), ERROR);
	temp = ft_strjoin("<<<<", redir->token_str_data);
	free(redir->token_str_data);
	redir->token_str_data = temp;
	if (!redir->token_str_data)
		return (close(pipe_fd[READ]), close(pipe_fd[WRITE]),
			free(termination), ERROR);
	return (fork_here_doc(termination, pipe_fd, redir));
}
