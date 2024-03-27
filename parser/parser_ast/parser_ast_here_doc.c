/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_here_doc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 04:56:10 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 11:33:49 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_result	here_doc_parent(char *termination, int pipe_fd[2], int pid,
	int std_in_pipe[2])
{
	int		child_has_exited;
	int		child_exit_status;
	char	*line;

	child_has_exited = 0;
	while (child_has_exited == 0 && !here_doc_exit_state(false, false))
	{
		child_has_exited = waitpid(pid, &child_exit_status, WNOHANG);
		write(1, "> ", 2 * -1 * (TESTER - 1));
		line = get_next_line(0, false);
		write(std_in_pipe[WRITE], line, ft_strlen(line));
		if (!line || (line && ft_strcmp(line, termination) == 0))
			break ;
		ft_free((void **)&line);
	}
	ft_free((void **)&line);
	free(termination);
	ft_close(&std_in_pipe[WRITE]);
	if (here_doc_exit_state(false, false))
		return (ft_close(&pipe_fd[READ]), ERROR);
	child_exit_status = WEXITSTATUS(child_exit_status);
	if (child_has_exited && child_exit_status)
		return (set_last_exit(child_exit_status),
			ft_close(&pipe_fd[READ]), ERROR);
	return (SUCCESS);
}

t_result	fork_here_doc(char *termination, int pipe_fd[2], t_redir *redir)
{
	int		pid;
	int		std_in_pipe[2];

	here_doc_exit_state(true, false);
	pipe(std_in_pipe);
	pid = fork();
	if (pid == -1)
	{
		close(std_in_pipe[READ]);
		close(std_in_pipe[WRITE]);
		free(termination);
		ft_close(&pipe_fd[WRITE]);
		ft_close(&pipe_fd[READ]);
		set_last_exit(errno);
		print_error(true, NULL, NULL, strerror(errno));
		return (ERROR);
	}
	else if (pid > 0)
	{
		close(std_in_pipe[READ]);
		ft_close(&pipe_fd[WRITE]);
		return (here_doc_parent(termination, pipe_fd, pid, std_in_pipe));
	}
	init_here_doc_child(pipe_fd, termination, redir, std_in_pipe);
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
