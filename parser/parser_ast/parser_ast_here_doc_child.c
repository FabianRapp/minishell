/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_here_doc_child.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 07:49:52 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 08:44:56 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_here_doc_child_data	*heredoc_chil_data_state(
	t_here_doc_child_data *new_state)
{
	static t_here_doc_child_data	*vars = NULL;

	if (new_state)
		vars = new_state;
	return (vars);
}

static char	*get_line(int fd)
{
	char	*line;
	char	*tmp;

	line = NULL;
	//if (!isatty(0))
	{
		tmp = get_next_line(fd);
		set_signals();
		line = ft_strtrim(tmp, "\n");
		free(tmp);
		set_ctrl_c_heredoc();
	}
	//else
	//	line = readline(">");
	return (line);
}

static bool	handle_success(char **line, int count, char *termination)
{
	char	*tmp;

	set_signals();
	if (!*line || !**line)
	{
		tmp = ft_strtrim(termination, "\n");
		ft_fprintf(2, "%s: warning: here-document at line %d delimited by "
			"end-of-file (wanted `%s')\n", SHELL_NAME, count, tmp);
		return (free(tmp), true);
	}
	ft_strjoin_inplace(line, "\n");
	if (ft_strcmp(*line, termination) == 0)
		return (ft_free((void **)line), true);
	set_ctrl_c_heredoc();
	return (false);
}

static t_result	parser_resolve_here_doc(t_here_doc_child_data *vars)
{
	vars->start = true;
	while (vars->line || vars->start)
	{
		vars->start = false;
		set_signals();
		ft_free((void **)&(vars->line));
		set_ctrl_c_heredoc();
		vars->line = get_line(vars->fd[READ]);
		vars->count = line_counter();
		if (here_doc_exit_state(false, false))
			return (ERROR);
		if (!vars->line && errno)
			return (set_last_exit(errno), ERROR);
		if (handle_success(&vars->line, vars->count, vars->termination) == true)
			return (SUCCESS);
		if (vars->expand_vars)
			vars->line = parser_expand_line(vars->line);
		if (ft_fprintf(vars->fd[WRITE], "%s", vars->line) == -1)
			return (set_last_exit(errno), ERROR);
	}
	if (errno)
		return (set_last_exit(errno), ERROR);
	return (set_last_exit(1), ERROR);
}

void	init_here_doc_child(int pipe_fd[2], char *termination, t_redir *redir, int std_in_pipe[2])
{
	t_here_doc_child_data	child_data;

	errno = 0;
	set_last_exit(0);
	close(pipe_fd[READ]);
	close(std_in_pipe[WRITE]);
	child_data.fd[WRITE] = pipe_fd[WRITE];
	child_data.fd[READ] = std_in_pipe[READ];
	child_data.termination = termination;
	child_data.expand_vars = !(redir->here_doc_literal);
	child_data.line = NULL;
	heredoc_chil_data_state(&child_data);
	set_ctrl_c_heredoc();
	if (parser_resolve_here_doc(&child_data) == ERROR)
		printf("child resolving returned WITH error (exit status: %d)\n", get_last_exit());
	else
		printf("child resolving returned with no error (exit status: %d)\n", get_last_exit());
	set_signals();
	close(std_in_pipe[READ]);
	free(child_data.line);
	close(pipe_fd[WRITE]);
	free(termination);
	exit(get_last_exit());
}
