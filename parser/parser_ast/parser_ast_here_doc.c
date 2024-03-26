/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_here_doc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 04:56:10 by frapp             #+#    #+#             */
/*   Updated: 2024/03/26 22:36:36 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static char	*get_line(void)
{
	char	*line;
	char	*tmp;

	line = NULL;
	if (!isatty(0))
	{
		tmp = get_next_line(0);
		line = ft_strtrim(tmp, "\n");
		free(tmp);
	}
	else
		line = readline(">");
	return (line);
}

static bool	handle_success(char **line, int count, char *termination)
{
	char	*tmp;

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
	return (false);
}

static t_result	parser_resolve_here_doc(char *termination,
	int pipe_fd[2], bool expand_vars)
{
	char	*line;
	bool	start;
	int		count;

	line = NULL;
	start = true;
	while (line || start)
	{
		start = false;
		free(line);
		line = get_line();
		count = line_counter();
		if (!line && errno)
			return (set_last_exit(errno), ERROR);
		if (handle_success(&line, count, termination) == true)
			return (SUCCESS);
		if (expand_vars)
			line = parser_expand_line(line);
		if (ft_fprintf(pipe_fd[WRITE], "%s", line) == -1)
			return (set_last_exit(errno), ERROR);
	}
	if (errno)
		return (free(line), set_last_exit(errno), ERROR);
	return (free(line), set_last_exit(1), ERROR);
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
	if (!redir->token_str_data || parser_resolve_here_doc(termination,
			pipe_fd,!(redir->here_doc_literal)) == ERROR)
		return (close(pipe_fd[READ]), close(pipe_fd[WRITE]),
			free(termination), ERROR);
	return (close(pipe_fd[WRITE]), free(termination), SUCCESS);
}
