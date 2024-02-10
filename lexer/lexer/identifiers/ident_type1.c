/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ident_type1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:29:01 by frapp             #+#    #+#             */
/*   Updated: 2024/02/09 23:35:16 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/lexer.h"
#include "../internals.h"

bool	env_var_type(t_lexer *lexer, t_token *token, bool *malloc_error)
{
	int	len;

	if (lexer->cur_char != '$')
		return (false);
	len = name_len((lexer->str) + lexer->position + 1);
	if (len == 0)
	{
		if ((lexer->str)[lexer->position + 1] == '$')
		{
			token->type = PID_REQUEST;
			len = 1;
			token->str_data = ft_strndup((lexer->str) + lexer->position + 1 , len);
			if (!token->str_data)
			{
				*malloc_error = true;
				return (false);
			}
			lexer->read_position = lexer->position + 1 + len;
			return (token->type);
		}
		if (ft_isdigit((lexer->str)[lexer->position + 1]))
		{
			lexer->read_position = lexer->position + 2;
			token->type = VOID;
			return (token->type);
		}
		return (false);
	}
	token->type = ENV_VAR;
	token->str_data = ft_strndup((lexer->str) + lexer->position + 1 , len);
	if (!token->str_data)
	{
		*malloc_error = true;
		return (false);
	}
	token->old_data = ft_strdup(token->str_data);
	if (!token->old_data)
	{
		*malloc_error = true;
		return (false);
	}
	lexer->read_position = lexer->position + 1 + len;
	return (token->type);
}

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

// // TODO:
// // handle >INT_MAX and < INT_MIN
// bool	integer_type(t_lexer *lexer, t_token *token)
// {
// 	if (lexer->cur_char == '0')
// 	{
// 		token->type = INTEGER;
// 		token->int_val = 0;
// 		return (token->type);
// 	}
// 	if (ft_atoi(lexer->str + lexer->position))
// 	{
// 		token->type = INTEGER;
// 		token->int_val = ft_atoi(lexer->str + lexer->position);
// 		while (ft_isdigit((lexer->str)[lexer->read_position]))
// 		{
// 			(lexer->read_position)++;
// 		}
// 		return (token->type);
// 	}
// 	return (token->type);
//}

bool	literal_type(t_lexer *lexer, t_token *token , bool *malloc_error)
{
	size_t	len;

	if (lexer->cur_char != '\'')
		return (false);
	while ((lexer->str)[lexer->read_position] && (lexer->str)[lexer->read_position] != '\'')
	{
		(lexer->read_position)++;
	}
	if (!(lexer->str)[lexer->read_position])
		return (token->type);
	(lexer->read_position)++; // +1 to remove the signle quote
	len = lexer->read_position - lexer->position - 2; // -2 to remove the quotes
	token->type = LITERAL;
	token->str_data =  ft_strndup(lexer->str + lexer->position + 1, len); // +1 to skip the initial quote
	if (!token->str_data)
	{
		*malloc_error = true;
		return (false);
	}
	return (token->type);
}

bool	interpreted_type(t_lexer *lexer, t_token *token, bool *malloc_error)
{
	size_t	len;

	if (lexer->cur_char != '\"')
		return (false);
	while ((lexer->str)[lexer->read_position] && (lexer->str)[lexer->read_position] != '\"')
	{
		(lexer->read_position)++;
	}
	if (!(lexer->str)[lexer->read_position])
		return (token->type);
	(lexer->read_position)++;
	len = lexer->read_position - lexer->position - 2; // -1 to remove the double quite
	token->type = INTERPRETED;
	token->str_data =  ft_strndup(lexer->str + lexer->position + 1, len); // +1 to remove the double quite
	if (!token->str_data)
	{
		*malloc_error = true;
		return (false);
	}
	return (token->type);
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
	return (token->type);
}

bool	subshell_type(t_lexer *lexer, t_token *token, bool *malloc_error)
{
	int	count_open;

	if (lexer->cur_char != '(')
		return (false);
	count_open = 1;
	while((lexer->str)[lexer->read_position] && count_open)
	{
		if ((lexer->str)[lexer->read_position] == '(')
			count_open++;
		else if ((lexer->str)[lexer->read_position] == ')')
			count_open--;
		(lexer->read_position)++;
	}
	if (count_open)
	{
		lexer->read_position = lexer->position + 1;
		return (false);
	}
	token->type = SUBSHELL;
	token->str_data = ft_strndup(lexer->str + lexer->position + 1, lexer->read_position - lexer->position - 2);
	if (!token->str_data)
	{
		*malloc_error = true;
		return (false);
	}
	return (token->type);
}

// has to run after all other typechecks
bool	literal_type2(t_lexer *lexer, t_token *token, bool *malloc_error)
{
	// TODO: fill token for $
	if (is_termination_char(lexer->cur_char)
		&& !(lexer->cur_char && lexer->cur_char == '$' //just '$' is a literal
			&& (ft_iswhitespace(lexer->str[lexer->position + 1]) || !(lexer->str)[lexer->position + 1]))) 
		return (0);
	while (!is_termination_char((lexer->str)[lexer->read_position]))
	{
		(lexer->read_position)++;
	}
	token->type = LITERAL;
	token->str_data = ft_strndup(lexer->str + lexer->position, lexer->read_position - lexer->position);
	if (!token->str_data)
	{
		*malloc_error = true;
		return (false);
	}
	return (token->type);
}

