/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:46:56 by frapp             #+#    #+#             */
/*   Updated: 2024/01/17 03:12:25 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

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
