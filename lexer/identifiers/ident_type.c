/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ident_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:29:01 by frapp             #+#    #+#             */
/*   Updated: 2024/03/09 02:40:29 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/lexer.h"
#include "../internals.h"

char	*extract_str_data(t_lexer *lexer)
{
	char	*str_data;
	int		len;

	len = lexer->read_position - lexer->position;
	str_data = ft_strndup(lexer->str + lexer->position, len);
	return (str_data);
}

void	type_token_with_void_check(t_token *token, t_type type)
{
	if (!token)
		return ;
	if (token->str_data == NULL || ft_strlen(token->str_data) == 0)
	{
		ft_free((void **)&(token->str_data));
		token->type = VOID;
	}
	else
		token->type = type;
}

void	basic_sign_type(t_lexer *lexer, t_token *token)
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
}

t_result	literal_type(t_lexer *lexer, t_token *token)
{
	if (lexer->cur_char != '\'')
		return (SUCCESS);
	while ((lexer->str)[lexer->read_position]
			&& (lexer->str)[lexer->read_position] != '\'')
	{
		(lexer->read_position)++;
	}
	if (!(lexer->str)[lexer->read_position])
	{
		print_error(true, "debug literal_type",
			NULL, "unexpected EOF while looking for matching `\'\'");
		print_error(false, NULL, NULL, "exit");
		set_last_exit(true);
		full_exit_status(true);
		return (ERROR);
	}
	lexer->position++;
	token->str_data = extract_str_data(lexer);
	if (!token->str_data)
		return (ERROR);
	lexer->read_position++;
	type_token_with_void_check(token, LITERAL);
	return (SUCCESS);
}

t_result	interpreted_type(t_lexer *lexer, t_token *token)
{
	if (lexer->cur_char != '\"')
		return (SUCCESS);
	while ((lexer->str)[lexer->read_position]
			&& (lexer->str)[lexer->read_position] != '\"')
	{
		(lexer->read_position)++;
	}
	if (!(lexer->str)[lexer->read_position])
	{
		ft_fprintf(2, "exit\n");
		print_error(true, "debug interpreted_type",
			NULL, "unexpected EOF while looking for matching `\"\'");
		exit(2);
	}
	lexer->position++;
	token->str_data = extract_str_data(lexer);
	if (!token->str_data)
		return (ERROR);
	lexer->read_position++;
	type_token_with_void_check(token, INTERPRETED);
	return (SUCCESS);
}
