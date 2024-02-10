/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 22:04:11 by frapp             #+#    #+#             */
/*   Updated: 2024/02/10 22:48:26 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/parser.h"
#include "../internals_parser.h"
#include "../../headers/lexer.h"

void	jump_to_start(t_parser **parser)
{
	while ((*parser)->p_type != T_EOF)
		(*parser) = (*parser)->next;
	(*parser) = (*parser)->next;
}

/*
	sets *node to the element before the removed node
*/
void	remove_parser_node(t_parser **node, bool free_tok)
{
	t_parser	*last;

	if (!node || !*node)
		return ;
	if (free_tok || (*node)->token->type == T_EOF || is_redir((*node)->token->type))
		free_token((*node)->token);
	last = *node;
	while (last->next != *node)
		last = last->next;
	last->next = (*node)->next;
	free(*node);
	if (last == (*node))
	{
		(*node) = NULL;
		return ;
	}
	(*node) = last;
}

// util for insert_token()
static t_parser	*fill_first_parser(t_parser **parser, t_token *token)
{
	*parser = ft_calloc(1, sizeof(t_parser));
	if (!*parser)
		return (free_token(token), NULL);
	(*parser)->p_type = token->type;
	(*parser)->token = token;
	(*parser)->next = *parser;
	return (*parser);
}

// returns the head
// appends a parser node and advances the parser to the new node
// (keeps cirular list)
// cleans up on error and returns false
t_parser	*insert_token(t_parser **parser, t_token *token)
{
	t_parser	*first;

	if (!*parser)
		return (fill_first_parser(parser, token));
	first = (*parser)->next;
	(*parser)->next = ft_calloc(1, sizeof(t_parser));
	if (!(*parser)->next)
	{
		(*parser)->next = first;
		return (free_parser_main(*parser, true), free_token(token), NULL);
	}
	(*parser)->next->token = token;
	(*parser)->next->p_type = token->type;
	*parser = (*parser)->next;
	(*parser)->next = first;
	return (first);
}
