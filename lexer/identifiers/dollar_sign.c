/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_sign.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 21:33:17 by frapp             #+#    #+#             */
/*   Updated: 2024/02/12 18:27:38 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/lexer.h"
#include "../internals.h"

// has to be called from dollar_lexing()
static t_result	pid_req(t_lexer *lexer, t_token *token)
{
	if (!((lexer->str)[lexer->position + 1] == '$'))
		return (ERROR);
	token->type = PID_REQUEST;
	lexer->read_position = lexer->position + 2;
	return (SUCCESS);
}

// has to be called from dollar_lexing()
static t_result	void_env_type(t_lexer *lexer, t_token *token)
{
	if (!ft_isdigit((lexer->str)[lexer->position + 1]))
		return (ERROR);
	lexer->read_position = lexer->position + 2;
	token->type = VOID;
	return (SUCCESS);
}

// has to be called from dollar_lexing()
// caller has to check for malloc fail
static t_result	is_dollar_literal(t_lexer *lexer, t_token *token)
{
	if (lexer->cur_char != '$')
		return (ERROR);
	if (ft_iswhitespace(lexer->str[lexer->position + 1])
			|| !(lexer->str)[lexer->position + 1])
	{
		token->type = LITERAL;
		token->str_data = ft_strdup("$");
		return (SUCCESS);
	}
	return (ERROR);
}

t_result	dollar_lexing(t_lexer *lexer, t_token *token)
{
	int	len;

	if (lexer->cur_char != '$')
		return (SUCCESS);
	if (pid_req(lexer, token) || void_env_type(lexer, token))
		return (SUCCESS);
	if (is_dollar_literal(lexer, token))
	{
		if (token->str_data)
			return (SUCCESS);
		return (ERROR);
	}
	len = name_len((lexer->str) + lexer->position + 1);
	if (len == 0)
		return (SUCCESS);
	token->type = ENV_VAR;
	token->str_data = ft_strndup((lexer->str) + lexer->position + 1 , len);
	if (!token->str_data)
		return (ERROR);
	token->old_data = ft_strdup(token->str_data);
	if (!token->old_data)
		return (ERROR);
	lexer->read_position = lexer->position + 1 + len;
	return (SUCCESS);
}
