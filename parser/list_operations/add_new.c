/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_new.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 22:13:59 by frapp             #+#    #+#             */
/*   Updated: 2024/02/10 22:43:00 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/parser.h"
#include "../../headers/lexer.h"
#include "../internals_parser.h"

// util for type_commands()
bool	insert_dummy(t_parser *parser)
{
	t_parser	*dummy;

	dummy = ft_calloc(1, sizeof(t_parser));
	if (!dummy)
		return (false);
	dummy->next = parser->next;
	dummy->p_type = COMMAND;
	dummy->token = new_dummy_token();
	if (!dummy->token)
		return (free(dummy), false);
	dummy->token->type = DUMMY_COMMAND;
	parser->next = dummy;
	return (true);
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
		return (NULL);
	while (!token || token->type != T_EOF)
	{
		token = next_new_token(&lexer);
		if (!token)
			return (free_parser_main(parser, true), NULL);
		if (token->type == VOID)
		{
			free_token(token);
			token = NULL;
			continue ;
		}
		insert_token(&parser, token);
	}
	if (parser)
		return (parser->next);
	return (NULL);
}
