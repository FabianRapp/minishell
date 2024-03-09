/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_redir.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 05:35:46 by frapp             #+#    #+#             */
/*   Updated: 2024/03/09 07:07:17 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/parser.h"
#include "../internals_parser.h"
#include "../../headers/lexer.h"

//for parser_resovle_here_doc
t_result	parser_resolve_here_doc(char *termination, int pipe_fd[2])
{
	char	*line;

	line = get_next_line(0);
	while (line)
	{
		if (ft_strcmp(line, termination) == 0)
		{
			ft_free((void **)&line);
			return (SUCCESS);
		}
		if (ft_fprintf(pipe_fd[WRITE], "%s", line) == -1)
			return (set_last_exit(errno), ERROR);
		ft_free((void **)&line);
		line = get_next_line(0);
	}
	if (errno)
		set_last_exit(errno);
	else
		set_last_exit(1);
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
	temp = NULL;
	if (redir->token_str_data)
		temp = ft_strjoin("<<<<", redir->token_str_data);
	free(redir->token_str_data);
	redir->token_str_data = temp;
	if (!redir->token_str_data
		|| parser_resolve_here_doc(termination, pipe_fd) == ERROR)
		return (close(pipe_fd[READ]), close(pipe_fd[WRITE]),
			free(termination), ERROR);
	return (close(pipe_fd[WRITE]), free(termination), SUCCESS);
}

t_result	append_redir(t_ast *ast_node, t_parser *args, t_redir **cur_redir)
{
	if (*cur_redir)
	{
		(*cur_redir)->next = ft_calloc(1, sizeof(t_redir));
		if (!(*cur_redir)->next)
			return (ERROR);
		*cur_redir = (*cur_redir)->next;
	}
	else
	{
		*cur_redir = ft_calloc(1, sizeof(t_redir));
		if (!*cur_redir)
			return (ERROR);
		ast_node->redir = *cur_redir;
	}
	(*cur_redir)->type = args->token->type;
	(*cur_redir)->token_str_data = ft_strdup(args->token->str_data);
	if (args->token->str_data && !(*cur_redir)->token_str_data)
		return (ERROR);
	(*cur_redir)->arg = append_arg(args->arg, (*cur_redir)->arg);
	if (!((*cur_redir)->arg))
		return (ERROR);
	(*cur_redir)->left_redir_arg = INIT_VAL;
	if (args->token->left_redir_arg != NULL)
		(*cur_redir)->left_redir_arg = ft_atoi(args->token->left_redir_arg);
	return (SUCCESS);
}
