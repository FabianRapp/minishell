/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:46:56 by frapp             #+#    #+#             */
/*   Updated: 2024/01/18 11:26:24 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/lexer.h"

// reads the next char into the lexer and updates indexes
void	read_char(t_lexer *lexer)
{
	if (((int)ft_strlen(lexer->str)) <= lexer->read_position)
	{
		lexer->cur_char = 0;
	}
	else
	{
		lexer->cur_char = (lexer->str)[lexer->read_position];
	}
	lexer->position = lexer->read_position;
	(lexer->read_position)++;
}

void	zero_token(t_token *token)
{
	token->type = UNKNOWN;
	token->int_val = 0;
	token->str = NULL;
	token->unknown = 0;
	token->last = NULL;
	token->next = NULL;
}

bool	is_termination(char c)
{
	if (c == 0 || c == '(' || c == ')' || c == '|' || c == '\'' || c == '\"' 
		|| c == '>' || c == '<' || c == '*' || c == '?'
		|| c == '$' || c == '&'
		|| ft_iswhitespace(c))
	{
		return (true);
	}
	return (false);
}

int	name_len(char *str)
{
	int		len;

	if (*str != '_' && !ft_isalpha(*str))
		return (0);
	len = 0;
	while (ft_isalnum(str[len]) || str[len] == '_')
	{
		len++;
	}
	return (len);
}
