/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ident_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 00:06:31 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 12:42:41 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static t_result	handle_redir_fd(t_lexer *lexer, t_token *token)
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

static bool	valid_redir_arg(t_lexer *lexer, t_type type)
{
	t_lexer	temp;

	temp = *lexer;
	if (type != HERE_DOC)
		read_char(lexer);
	while (ft_iswhitespace_minishell(lexer->cur_char))
		read_char(lexer);
	if (!is_redir_terminator_char(lexer->cur_char))
	{
		*lexer = temp;
		return (true);
	}
	print_error_redir_arg(lexer);
	*lexer = temp;
	return (false);
}

static t_result	lexer_here_doc(t_lexer *lexer, t_token *token)
{
	if (token->type != HERE_DOC)
		return (SUCCESS);
	while (ft_iswhitespace_minishell(lexer->cur_char))
		read_char(lexer);
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

static void	ft_redir_in(t_lexer *lexer, t_token *token)
{
	token->type = REDIR_IN;
	if (lexer->str[lexer->read_position] == '<')
	{
		read_char(lexer);
		if (lexer->str[lexer->read_position] == '<')
		{
			read_char(lexer);
			token->type = HERE_STR;
		}
		else
		{
			read_char(lexer);
			token->type = HERE_DOC;
		}
	}
	else if (lexer->str[lexer->read_position] == '>')
	{
		read_char(lexer);
		token->type = REDIR_IN_OUT;
	}
}

t_result	redir_type(t_lexer *lexer, t_token *token, bool recursive_call)
{
	if (handle_redir_fd(lexer, token) == ERROR)
		return (ERROR);
	if (lexer->cur_char != '<' && lexer->cur_char != '>')
		return (SUCCESS);
	if (lexer->cur_char == '<')
		ft_redir_in(lexer, token);
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
