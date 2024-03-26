/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_redir.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 05:35:46 by frapp             #+#    #+#             */
/*   Updated: 2024/03/26 02:57:02 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/parser.h"
#include "../internals_parser.h"
#include "../../headers/lexer.h"

t_result	parser_resovle_here_str(t_redir *redir)
{
	int				pipe_fd[2];
	char			*str;
	char			*temp;

	str = redir->arg->name->token->str_data;
	redir->arg->name->token->str_data = NULL;
	if (pipe(pipe_fd) == -1)
		return (set_last_exit(errno), free(str), ERROR);
	redir->token_str_data = ft_itoa(pipe_fd[READ]);
	if (!redir->token_str_data)
		return (close(pipe_fd[READ]), close(pipe_fd[WRITE]),
			free(str), ERROR);
	temp = ft_strjoin("<<<<", redir->token_str_data);
	free(redir->token_str_data);
	if (!temp)
		return (close(pipe_fd[READ]), close(pipe_fd[WRITE]),
			free(str), ERROR);
	redir->token_str_data = temp;
	if (write (pipe_fd[WRITE], str, ft_strlen(str)) == -1)
		return (close(pipe_fd[READ]), close(pipe_fd[WRITE]),
			free(str), ERROR);
	if (write (pipe_fd[WRITE], "\n", 1) == -1)
		return (close(pipe_fd[READ]), close(pipe_fd[WRITE]),
			free(str), ERROR);
	return (close(pipe_fd[WRITE]), free(str), SUCCESS);
}

static t_result	init_append_redir(t_ast *ast_node, t_redir **cur_redir)
{
	if (*cur_redir)
	{
		(*cur_redir)->next = ft_calloc(1, sizeof(t_redir));
		if (!(*cur_redir)->next)
			return (set_errno_as_exit(ast_node, false), ERROR);
		*cur_redir = (*cur_redir)->next;
	}
	else
	{
		*cur_redir = ft_calloc(1, sizeof(t_redir));
		ast_node->redir = *cur_redir;
		if (!*cur_redir)
			return (set_errno_as_exit(ast_node, false), ERROR);
	}
	return (SUCCESS);
}

t_result	append_redir(t_ast *ast_node, t_parser *args, t_redir **cur_redir)
{
	if (init_append_redir(ast_node, cur_redir) == ERROR)
		return (ERROR);
	(*cur_redir)->type = args->token->type;
	(*cur_redir)->token_str_data = ft_strdup(args->token->str_data);
	if (args->token->str_data && !(*cur_redir)->token_str_data)
		return (ERROR);
	(*cur_redir)->arg = append_arg(args->arg, (*cur_redir)->arg);
	if (!((*cur_redir)->arg) && (*cur_redir)->type != HERE_DOC)
		return (ERROR);
	(*cur_redir)->here_doc_literal = args->token->here_doc_arg_literal;
	(*cur_redir)->left_redir_arg = INIT_VAL;
	if (args->token->left_redir_arg != NULL)
		(*cur_redir)->left_redir_arg = ft_atoi(args->token->left_redir_arg);
	return (SUCCESS);
}
