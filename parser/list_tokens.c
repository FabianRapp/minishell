/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 09:26:13 by frapp             #+#    #+#             */
/*   Updated: 2024/01/22 18:08:02 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"
#include "../headers/minishell.h"
#include "internals_parser.h"

bool	add_token(t_parser **parser, t_token *token)
{
	if (!token || !parser)
	{
		printf ("error add_token\n");
		return (false);
	}
	if (!*parser)
		*parser = ft_calloc(1, sizeof(t_parser));
	if (!*parser)
		return (cleanup(), false);
	if (!(*parser)->token)
	{
		if ((*parser)->last || (*parser)->next)
		{
			printf("error in add_token\n");
			return (false);
		}
		(*parser)->type = token->type;
		(*parser)->token = token;
		return (true);
	}
	(*parser)->next = ft_calloc(1, sizeof(t_parser));
	if (!(*parser)->next)
		return (cleanup(), false);
	(*parser)->next->token = token;
	(*parser)->next->type = token->type;
	(*parser)->next->last_type = (*parser)->type;
	(*parser)->next->last = *parser;
	(*parser)->next->next_type = (*parser)->next_type;
	(*parser)->next_type = token->type;
	(*parser) = (*parser)->next;
	return (true);
}

// each node has to be freed and each tokens values as a normal token would
t_parser	*link_parser(char *str)
{
	t_token			*token;
	t_parser		*first;
	t_lexer			lexer;
	t_parser		*parser;

	first = NULL;
	token = NULL;
	parser = NULL;
	lexer = new_lexer(str);
	while (!token || token->type != T_EOF)
	{
		token = next_new_token(&lexer);
		if (!add_token(&parser, token))
			return (NULL);
		if (!first)
		{
			first = parser;
			if (parser->type == T_EOF)
				return (parser);
		}
	}
	parser->type = parser->token->type;
	parser->last_type = parser->last->type;
	parser->next = first;
	first->last = parser;
	first->last_type = parser->type;
	parser->next_type = first->type;
	return (first);
}

t_parser	*init_parser(char *str)
{
	t_parser	*parser;

	parser = link_parser(str);
	
	return (parser);
}

bool	continue_parser(t_parser **parser)
{
	*parser = (*parser)->next;
	if ((*parser)->type == T_EOF)
		return (false);
	return (true);
}

bool	reverse_parser(t_parser **parser)
{
	*parser = (*parser)->last;
	if ((*parser)->type == T_EOF)
		return (false);
	return (true);
}

void	free_token(t_token *token)
{
	if (!token)
		return ;
	if (token->str) ////??? WHY CAN I NOT FREE THIS IF ITS NOT NULL
		free(token->str);
	free(token);
}
/*
TODO: TEST  THIS
	- returns 1 on success, unless:
	- returns 0 if the token list is empty afterwards (can be changed if needed)
	- returns -1 if the list is corrupted and does not free in that case
		(does not activly check for list intagret)
	sets *node to the element before the removed node
*/
int	remove_parser_node(t_parser **node)
{
	t_parser	*temp;

	if (!node || ! (*node))
		return (0);
	if (*node == (*node)->last || *node == (*node)->next)
		return (-1);
	// after this only one or none tokens left in the list
	if ((*node)->last == (*node)->next)
	{
		if ((*node)->last == 0 || (*node)->next == 0) // list empty afterwards
		{

			if (!(*node))
				return (-1);
			if ((*node == (*node)->last) || ((*node == (*node)->next)))
				return (-1);
			free_token((*node)->token);
			free(*node);
			(*node) = NULL;
			return (0);
		}
		free_token((*node)->token);
		temp =  (*node)->last;
		(*node)->next->last = (*node)->last;
		(*node)->last->next = (*node)->next;
		free(*node);
		(*node) = temp;
		if (*node == (*node)->last)
		{
			(*node)->next = NULL;
			(*node)->last = NULL;
		}
		return (true);
	}
	// duo to last condtion this means only one is null
	if (!(*node)->last || !(*node)->next)
		return (-1);
	// now only cases where there are diffrent last and next elements left
	(*node)->last->next_type = (*node)->next_type;
	(*node)->next->last_type = (*node)->last_type;
	(*node)->last->next = (*node)->next;
	(*node)->next->last = (*node)->last;
	temp = (*node)->last;
	free_token((*node)->token);
	free(*node); //works
	*node = temp;
	if (*node == (*node)->next || *node == (*node)->next)
	{
		(*node)->next = NULL;
		(*node)->last = NULL;
	}
	return (1);
}

bool	insert_token(t_parser **insert_after, t_token *insert_token)
{
	t_parser	*insert_node;

	if (!insert_token)
		return (true);
	if (!insert_after)
		return (false);
	insert_node = ft_calloc(1, sizeof(t_parser));
	if (!insert_node)
		return (false);
	insert_node->token = insert_token;
	insert_node->type = insert_token->type;
	if (!*insert_after)
	{
		*insert_after = insert_node;
		return (true);
	}
	insert_node->next = (*insert_after)->next;
	insert_node->next_type = (*insert_after)->next_type;
	if (insert_node->next)
	{
		insert_node->next->last = insert_node;
		insert_node->next->last_type = insert_node->type;
	}
	(*insert_after)->next = insert_node;
	(*insert_after)->next_type = insert_node->type;
	return (true);
}

