/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_sign.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 21:33:17 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 12:42:46 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static t_result	pid_req(t_lexer *lexer, t_token *token)
{
	if (!((lexer->str)[lexer->position + 1] == '$'))
		return (ERROR);
	token->type = PID_REQUEST;
	lexer->read_position = lexer->position + 2;
	return (SUCCESS);
}

static t_result	is_dollar_literal(t_lexer *lexer, t_token *token)
{
	if (lexer->cur_char != '$')
		return (ERROR);
	if (ft_iswhitespace_minishell(lexer->str[lexer->position + 1])
		|| !(lexer->str)[lexer->position + 1]
		|| lexer->str[lexer->position + 1] == '/')
	{
		token->type = LITERAL;
		token->str_data = ft_strdup("$");
		return (SUCCESS);
	}
	return (ERROR);
}

static	t_result	hande_quote(t_lexer *lexer, t_token *token)
{
	read_char(lexer);
	read_char(lexer);
	while (lexer->cur_char != '\'' && lexer->cur_char != '\"')
	{
		if (!ft_strjoin_inplace_char(&(token->str_data), lexer->cur_char))
			return (ERROR);
		read_char(lexer);
	}
	token->type = INTERPRETED;
	return (SUCCESS);
}

static t_result	dollar_lexing_rest(t_lexer *lexer, t_token *token)
{
	int	len;

	len = name_len((lexer->str) + lexer->position + 1);
	if (ft_isdigit(lexer->str[lexer->position + 1]))
		len = 1;
	token->type = ENV_VAR;
	token->str_data = ft_strndup((lexer->str) + lexer->position + 1, len);
	token->old_data = ft_strndup((lexer->str) + lexer->position + 1, len);
	if (!token->str_data || !token->old_data)
		return (ERROR);
	lexer->read_position = lexer->position + 1 + len;
	read_char(lexer);
	return (SUCCESS);
}

t_result	dollar_lexing(t_lexer *lexer, t_token *token)
{
	if (lexer->cur_char != '$')
		return (SUCCESS);
	if (pid_req(lexer, token))
		return (read_char(lexer), SUCCESS);
	if (is_dollar_literal(lexer, token))
	{
		if (token->str_data)
			return (read_char(lexer), SUCCESS);
		return (ERROR);
	}
	if (lexer->str[lexer->position + 1] == '\''
		|| lexer->str[lexer->position + 1] == '\"')
	{
		if (hande_quote(lexer, token) == ERROR)
			return (ERROR);
		if (token->type)
			return (read_char(lexer), SUCCESS);
	}
	return (dollar_lexing_rest(lexer, token));
}
