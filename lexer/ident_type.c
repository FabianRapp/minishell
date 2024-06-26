/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ident_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:29:01 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 13:43:02 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static char	*extract_str_data(t_lexer *lexer)
{
	char	*str_data;
	int		len;

	len = lexer->read_position - lexer->position;
	str_data = ft_strndup(lexer->str + lexer->position, len);
	return (str_data);
}

static void	set_type_and_position(t_lexer *lexer, t_token *token, t_type type)
{
	token->type = type;
	lexer->read_position = lexer->position + 2;
}

void	basic_sign_type(t_lexer *lexer, t_token *token)
{
	if (lexer->cur_char == 0)
		token->type = T_EOF;
	else if (ft_iswhitespace_minishell(lexer->cur_char))
		token->type = WHITE_SPACE;
	else if (lexer->cur_char == '&' && (lexer->str)[lexer->position + 1] == '&')
		set_type_and_position(lexer, token, AND);
	else if (lexer->cur_char == '|' && (lexer->str)[lexer->position + 1] == '|')
		set_type_and_position(lexer, token, OR);
	else if (lexer->cur_char == '|' && lexer->last_char != '\\')
		token->type = PIPE;
	else if (lexer->cur_char == '$' && (lexer->str)[lexer->position + 1] == '?')
	{
		token->type = EXIT_STATUS_REQUEST;
		lexer->read_position = lexer->position + 2;
	}
	else if (lexer->cur_char == ';' && lexer->last_char != '\\')
		token->type = SEMICOL;
	else
		return ;
	read_char(lexer);
}

t_result	literal_type(t_lexer *lexer, t_token *token)
{
	if (lexer->cur_char != '\'')
		return (SUCCESS);
	token->type = LITERAL;
	token->str_data = ft_calloc(1, 1);
	while ((lexer->str)[lexer->read_position]
			&& (lexer->str)[lexer->read_position] != '\'')
	{
		if (!ft_strjoin_inplace_char(&(token->str_data),
				lexer->str[lexer->read_position]))
			return (ERROR);
		(lexer->read_position)++;
	}
	if (!(lexer->str)[lexer->read_position])
	{
		print_error(true, NULL, NULL,
			"unexpected EOF while looking for matching `\'\'");
		set_last_exit(2);
		return (ERROR);
	}
	lexer->read_position++;
	read_char(lexer);
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
		print_error(true, NULL,
			NULL, "unexpected EOF while looking for matching `\"\'");
		set_last_exit(2);
		return (ERROR);
	}
	lexer->position++;
	token->str_data = extract_str_data(lexer);
	if (!token->str_data)
		return (ERROR);
	lexer->read_position++;
	token->type = INTERPRETED;
	read_char(lexer);
	return (SUCCESS);
}
