/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ident_type1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:29:01 by frapp             #+#    #+#             */
/*   Updated: 2024/02/10 21:34:52 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/lexer.h"
#include "../internals.h"

bool	basic_sign_type(t_lexer *lexer, t_token *token)
{
	if (lexer->cur_char == 0)
		token->type = T_EOF;
	else if (ft_iswhitespace(lexer->cur_char))
		token->type = WHITE_SPACE;
	else if (lexer->cur_char == '&' && (lexer->str)[lexer->position + 1] == '&')
	{
		token->type = AND;
		lexer->read_position = lexer->position + 2;
	}
	else if (lexer->cur_char == '|' && (lexer->str)[lexer->position + 1] == '|')
	{
		token->type = OR;
		lexer->read_position = lexer->position + 2;
	}
	else if (lexer->cur_char == '|')
		token->type = PIPE;
	else if (lexer->cur_char == '$' && (lexer->str)[lexer->position + 1] == '?')
	{
		token->type = EXIT_STATUS_REQUEST;
		lexer->read_position = lexer->position + 2;
	}
	else if (lexer->cur_char == '*')
		token->type = WILDCARD;
	return (token->type);
}

bool	literal_type(t_lexer *lexer, t_token *token)
{
	size_t	len;

	if (lexer->cur_char != '\'')
		return (true);
	while ((lexer->str)[lexer->read_position] && (lexer->str)[lexer->read_position] != '\'')
	{
		(lexer->read_position)++;
	}
	if (!(lexer->str)[lexer->read_position])
	{
		ft_fprintf(2, "exit\n");
		print_error(true, NULL, NULL, "unexpected EOF while looking for matching `\'\'");
		exit(2);
	}
	(lexer->read_position)++; // +1 to remove the signle quote
	len = lexer->read_position - lexer->position - 2; // -2 to remove the quotes
	token->type = LITERAL;
	token->str_data =  ft_strndup(lexer->str + lexer->position + 1, len); // +1 to skip the initial quote
	if (!token->str_data)
		return (false);
	return (true);
}

bool	interpreted_type(t_lexer *lexer, t_token *token)
{
	size_t	len;

	if (lexer->cur_char != '\"')
		return (true);
	while ((lexer->str)[lexer->read_position] && (lexer->str)[lexer->read_position] != '\"')
	{
		(lexer->read_position)++;
	}
	if (!(lexer->str)[lexer->read_position])
	{
		ft_fprintf(2, "exit\n");
		print_error(true, NULL, NULL, "unexpected EOF while looking for matching `\"\'");
		exit(2);
	}
	(lexer->read_position)++;
	len = lexer->read_position - lexer->position - 2; // -1 to remove the double quite
	token->type = INTERPRETED;
	token->str_data =  ft_strndup(lexer->str + lexer->position + 1, len); // +1 to remove the double quite
	if (!token->str_data)
		return (false);
	return (true);
}

bool	redir_type(t_lexer *lexer, t_token *token)
{
	if (lexer->cur_char == '<')
	{
		if ((lexer->str)[lexer->position + 1] == '<')
		{
			token->type = HERE_DOC;
			lexer->read_position += 1;
		}
		else
			token->type = REDIR_IN;
	}
	else if (lexer->cur_char == '>')
	{
		if ((lexer->str)[lexer->position + 1] == '>')
		{
			token->type = REDIR_APPEND;
			lexer->read_position += 1;
		}
		else
			token->type = REDIR_OUT;
	}
	return (true);
}

// has to run after all other typechecks
bool	literal_type2(t_lexer *lexer, t_token *token)
{

	if (is_termination_char(lexer->cur_char))
		return (true);
	while (!is_termination_char((lexer->str)[lexer->read_position]))
	{
		(lexer->read_position)++;
	}
	token->type = LITERAL;
	token->str_data = ft_strndup(lexer->str + lexer->position, lexer->read_position - lexer->position);
	if (!token->str_data)
		return (false);
	return (true);
}
