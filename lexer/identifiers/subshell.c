/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 21:34:29 by frapp             #+#    #+#             */
/*   Updated: 2024/02/10 21:34:50 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/lexer.h"
#include "../internals.h"

// TODO need to reade new lexer process and keep track if there is invalid syntax within the subshell
bool	subshell_type(t_lexer *lexer, t_token *token)
{
	int		count_open;

	if (lexer->cur_char != '(')
		return (true);
	if ((lexer->str)[lexer->read_position] == ')')
	{
		print_error(true, NULL, NULL, "syntax error near unexpected token `)\'");
		return (false);
	}
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
		print_error(true, NULL, "syntax error", " unexpected end of file");
		ft_fprintf(2, "exit\n");
		exit(2);
	}
	token->type = SUBSHELL;
	token->str_data = ft_strndup(lexer->str + lexer->position + 1, lexer->read_position - lexer->position - 2);
	if (!token->str_data)
		return (false);
	return (true);
}
