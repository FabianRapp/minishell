/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_here_doc_child.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 07:49:52 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 07:57:49 by frapp            ###   ########.fr       */
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

static char	*get_line(void)
{
	char	*line;
	char	*tmp;

	line = NULL;
	if (!isatty(0))
	{
		tmp = get_next_line(0);
		set_signals();
		line = ft_strtrim(tmp, "\n");
		free(tmp);
		set_ctrl_c_heredoc();
	}
	else
		line = readline(">");
	return (line);
}

static bool	handle_success(char **line, int count, char *termination)
{
	char	*tmp;

	set_signals();
	if (!*line)
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
		vars->line = get_line();
		vars->count = line_counter();
		if (here_doc_exit_state(false, false))
			return (ERROR);
		if (!vars->line && errno)
			return (set_last_exit(errno), ERROR);
		if (handle_success(&vars->line, vars->count, vars->termination) == true)
			return (SUCCESS);
		if (vars->expand_vars)
			vars->line = parser_expand_line(vars->line);
		if (ft_fprintf(vars->fd, "%s", vars->line) == -1)
			return (set_last_exit(errno), ERROR);
	}
	if (errno)
		return (set_last_exit(errno), ERROR);
	return (set_last_exit(1), ERROR);
}

void	init_here_doc_child(int pipe_fd[2], char *termination, t_redir *redir)
{
	t_here_doc_child_data	child_data;

	errno = 0;
	set_last_exit(0);
	close(pipe_fd[READ]);
	child_data.fd = pipe_fd[WRITE];
	child_data.termination = termination;
	child_data.expand_vars = !(redir->here_doc_literal);
	child_data.line = NULL;
	heredoc_chil_data_state(&child_data);
	set_ctrl_c_heredoc();
	parser_resolve_here_doc(&child_data);
	set_signals();
	free(child_data.line);
	close(pipe_fd[WRITE]);
	free(termination);
	exit(get_last_exit());
}
