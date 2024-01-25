/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:46:56 by frapp             #+#    #+#             */
/*   Updated: 2024/01/24 23:53:39 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/lexer.h"

// reads the next char into the lexer and updates indexes
void	read_char(t_lexer *lexer)
{
	lexer->cur_char = (lexer->str)[lexer->read_position];
	lexer->position = lexer->read_position;
	if (lexer->read_position < ((int)ft_strlen(lexer->str)))
		(lexer->read_position)++;
}

void	init_token(t_token *token, t_lexer *lexer)
{
	token->type = 0;
	token->str_data = NULL;
	token->unknown = 0;
	token->input_str = lexer->str;
	token->input_position = lexer->position;
}

int	name_len(char *str)
{
	int		len;

	// /if (*str && *str != '_' && !ft_isalnum(*str))
	if (*str && *str != '_' && !ft_isalpha(*str))
		return (0);
	len = 1;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
	{
		len++;
	}
	return (len);
}
