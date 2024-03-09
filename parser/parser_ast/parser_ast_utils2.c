/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 06:03:37 by frapp             #+#    #+#             */
/*   Updated: 2024/03/09 07:08:32 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/parser.h"
#include "../internals_parser.h"
#include "../../headers/lexer.h"

// for append_arg()
static t_arg	*get_cur_arg(t_arg *head_arg)
{
	t_arg	*cur;

	if (!head_arg)
		cur = ft_calloc(1, sizeof(t_arg));
	else
	{
		cur = head_arg;
		while (cur->next)
			cur = cur->next;
		cur->next = ft_calloc(1, sizeof(t_arg));
		cur = cur->next;
	}
	return (cur);
}

// returns the head of the token list, NULL on malloc fail
// assums the given list to be the head
t_arg	*append_arg(t_parser *parser, t_arg *head_arg)
{
	t_arg	*cur;

	if (!parser)
		return (NULL);
	cur = get_cur_arg(head_arg);
	if (!cur)
		return (free_arg_list(head_arg), NULL);
	if (!head_arg)
		head_arg = cur;
	cur->name = ft_calloc(1, sizeof(t_arg));
	if (!cur->name)
		return (free_arg_list(head_arg), NULL);
	cur->name->token = parser->token;
	parser->token = NULL;
	if (parser->rest_name)
	{
		cur->name->next = extract_token_list(parser->rest_name, RECURSIVE_CALL);
		if (!cur->name->next)
		{
			return (free_arg_list(head_arg), NULL);
		}
	}
	cur->type = cur->name->token->type;
	return (head_arg);
}
