/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ident_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 00:06:31 by frapp             #+#    #+#             */
/*   Updated: 2024/03/22 01:32:56 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/lexer.h"
#include "../internals.h"

// util for redir_type
char	*get_potential_fd(t_lexer *lexer)
{
	char	*left_redir_arg;
	t_lexer	lexer_backup;

	lexer_backup = *lexer;
	left_redir_arg = NULL;
	while (ft_isdigit(lexer->cur_char))
	{
		if (!ft_strjoin_inplace_char(&left_redir_arg, lexer->cur_char))
			return (NULL);
		read_char(lexer);
	}
	return (left_redir_arg);
}

t_result	handle_redir_fd(t_lexer *lexer, t_token *token)
{
	t_lexer	lexer_backup;

	if (ft_isdigit(lexer->cur_char))
	{
		lexer_backup = *lexer;
		token->left_redir_arg = get_potential_fd(lexer);
		if (!token->left_redir_arg)
			return (ERROR);
		token->left_redir_arg = check_limis_potential_fd
			(token->left_redir_arg, lexer, lexer_backup);
	}
	return (SUCCESS);
}

bool	valid_redir_arg(t_lexer *lexer, t_type type)
{
	t_token	*error_token;
	int		temp_pipe[2];
	int		base_std_err;
	t_lexer	temp;

	temp = *lexer;
	if (type != HERE_DOC)
		read_char(lexer);
	while (ft_iswhitespace(lexer->cur_char))
		read_char(lexer);
	if (!is_redir_terminator_char(lexer->cur_char))
	{
		*lexer = temp;
		return (true);
	}
	set_last_exit(2);
	base_std_err = dup(2);
	(pipe(temp_pipe) * 0 || close(temp_pipe[READ]) * 0
		|| dup2(temp_pipe[WRITE], 2) * 0 || close(temp_pipe[WRITE]));
	error_token = next_new_token(lexer, true);
	dup2(base_std_err, 2);
	if (error_token)
		print_error(true, NULL, NULL, type_to_str(error_token->type));
	*lexer = temp;
	return (close(base_std_err), free_token(error_token), false);
}

t_result	lexer_here_doc(t_lexer *lexer, t_token *token)
{
	if (token->type != HERE_DOC)
		return (SUCCESS);
	while (ft_iswhitespace(lexer->cur_char))
	{
		read_char(lexer);
	}
	while (!is_redir_terminator_char(lexer->cur_char))
	{
		if (lexer->cur_char == '\'')
			token->here_doc_arg_literal = true;
		else if (lexer->cur_char == '\"')
			token->here_doc_arg_literal = false;
		else if (!ft_strjoin_inplace_char(&(token->str_data), lexer->cur_char))
			return (ERROR);
		read_char(lexer);
	}
	if (lexer->cur_char == '\'' || lexer->cur_char == '\"')
		read_char(lexer);
	return (SUCCESS);
}

t_result	redir_type(t_lexer *lexer, t_token *token, bool recursive_call)
{
	if (handle_redir_fd(lexer, token) == ERROR)
		return (ERROR);
	if (lexer->cur_char != '<' && lexer->cur_char != '>')
		return (SUCCESS);
	if (lexer->cur_char == '<')
	{
		token->type = REDIR_IN;
		if (lexer->str[lexer->read_position] == '<')
		{
			read_char(lexer);
			if (lexer->str[lexer->read_position] == '<')
				(read_char(lexer), token->type = HERE_STR);
			else
				(read_char(lexer), token->type = HERE_DOC);
		}
		else if (lexer->str[lexer->read_position] == '>')
			(read_char(lexer), token->type = REDIR_IN_OUT);
	}
	else if (lexer->cur_char == '>')
	{
		token->type = REDIR_OUT;
		if (lexer->str[lexer->read_position] == '>')
			(read_char(lexer), token->type = REDIR_APPEND);
	}
	if (!recursive_call && !valid_redir_arg(lexer, token->type))
		return (ERROR);
	if (token->type != HERE_DOC)
		return (read_char(lexer), SUCCESS);
	return (lexer_here_doc(lexer, token));
}
