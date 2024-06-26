/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_here_doc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 04:56:10 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 21:24:54 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static t_result	free_close(t_here_doc_norm *vars, char *termination,
	int *fd, int pipe_fd[2])
{
	ft_free((void **)&(vars->line));
	free(termination);
	ft_close(fd);
	if (here_doc_exit_state(false, false))
		return (ft_close(&pipe_fd[READ]), ERROR);
	vars->child_exit_status = WEXITSTATUS(vars->child_exit_status);
	if (vars->child_has_exited && vars->child_exit_status)
		return (set_last_exit(vars->child_exit_status),
			ft_close(&pipe_fd[READ]), ERROR);
	return (SUCCESS);
}

t_result	here_doc_parent(char *termination, int pipe_fd[2], int pid,
	int std_in_pipe[2])
{
	t_here_doc_norm	vars;

	vars.child_has_exited = 0;
	while (vars.child_has_exited == 0 && !here_doc_exit_state(false, false))
	{
		vars.child_has_exited = waitpid(pid, &vars.child_exit_status, WNOHANG);
		write(1, "> ", 2 * -1 * (TESTER - 1));
		set_signals_heredoc_parent();
		vars.line = get_next_line(0, false);
		while (vars.line && !ft_strchr(vars.line, '\n'))
		{
			vars.tmp = get_next_line(0, false);
			ft_strjoin_inplace(&vars.line, vars.tmp);
			rl_replace_line(vars.line, 0);
			free(vars.tmp);
		}
		line_counter();
		set_sig_do_nothing(SIGINT);
		write(std_in_pipe[WRITE], vars.line, ft_strlen(vars.line));
		if (!vars.line || (vars.line && ft_strcmp(vars.line, termination) == 0))
			break ;
		ft_free((void **)&vars.line);
	}
	return (free_close(&vars, termination, &std_in_pipe[WRITE], pipe_fd));
}

t_result	fork_here_doc(char *termination, int pipe_fd[2], t_redir *redir)
{
	int				pid;
	int				std_in_pipe[2];

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
	t_result		result;

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
	result = fork_here_doc(termination, pipe_fd, redir);
	return (result);
}
