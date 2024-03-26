/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_new.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 22:13:59 by frapp             #+#    #+#             */
/*   Updated: 2024/03/26 17:40:26 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/parser.h"
#include "../../headers/lexer.h"
#include "../internals_parser.h"

// t_result	insert_dummy_after(t_parser *parser)
// {
// 	t_parser	*dummy;

// 	dummy = ft_calloc(1, sizeof(t_parser));
// 	if (!dummy)
// 		return (ERROR);
// 	dummy->next = parser->next;
// 	dummy->p_type = COMMAND;
// 	dummy->token = new_dummy_token();
// 	if (!dummy->token)
// 		return (free(dummy), ERROR);
// 	dummy->token->type = DUMMY_COMMAND;
// 	parser->next = dummy;
// 	return (SUCCESS);
// }

t_result	insert_dummy_here(t_parser *parser)
{
	t_parser	*next;

	next = ft_calloc(1, sizeof(t_parser));
	if (!next)
		return (ERROR);
	*next = *parser;
	parser->next = next;
	parser->p_type = COMMAND;
	parser->token = new_dummy_token();
	if (!parser->token)
	{
		parser->next = parser->next->next;
		return (free(next), ERROR);
	}
	return (SUCCESS);
}

// creates a cricular singular linked list
// each node has to be freed and each tokens values as a normal token would
t_parser	*init_parser(char *str)
{
	t_token			*token;
	t_lexer			lexer;
	t_parser		*parser;

	token = NULL;
	parser = NULL;
	lexer = new_lexer(str);
	if (!lexer.str || lexer.cur_char == 0)
		return (free(lexer.str), NULL);
	while (!token || token->type != T_EOF)
	{
		token = next_new_token(&lexer, false);
		if (!token)
			return (free(lexer.str), free_parser_main(parser, true), NULL);
		if (token->type == VOID)
		{
			free_token(token);
			token = NULL;
			continue ;
		}
		insert_token(&parser, token);
	}
	if (parser)
		return (free(lexer.str), parser->next);
	return (free(lexer.str), NULL);
}
