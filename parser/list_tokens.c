/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 09:26:13 by frapp             #+#    #+#             */
/*   Updated: 2024/01/25 16:05:59 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parser.h"
#include "../headers/lexer.h"
#include "internals_parser.h"

bool	insert_token(t_parser **parser, t_token *token)
{
	t_parser	*head;

	if (!token || !parser)
	{
		printf ("error add_token_back\n");
		return (false);
	}
	if (!*parser)
		*parser = ft_calloc(1, sizeof(t_parser));
	if (!*parser)
		return (cleanup(), false);
	if (!(*parser)->token)
	{
		if ((*parser)->next)
		{
			printf("error in add_token_back\n");
			return (false);
		}
		(*parser)->p_type = token->type;
		(*parser)->token = token;
		(*parser)->next = (*parser);
		return (true);
	}
	head = (*parser)->next;
	(*parser)->next = ft_calloc(1, sizeof(t_parser));
	if (!(*parser)->next)
		return (cleanup(), false);
	(*parser)->next->token = token;
	(*parser)->next->p_type = token->type;
	(*parser) = (*parser)->next;
	(*parser)->next = head;
	return (true);
}

// creates a cricular singular linked list
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
		token = next_new_token(&lexer); // null check
		if (token->type == VOID)
		{
			free_token(token);
			continue ;
		}
		if (!insert_token(&parser, token))// null check
			return (NULL);
		if (!first)
		{
			first = parser;
			if (parser->p_type == T_EOF)
				return (parser);
		}
	}
	if (!parser)
		return (NULL);
	parser->p_type = parser->token->type;
	parser->next = first;
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
	if ((*parser)->p_type == T_EOF)
		return (false);
	return (true);
}

void	jump_to_start(t_parser **parser)
{
	while ((*parser)->p_type != T_EOF)
		(*parser) = (*parser)->next;
	(*parser) = (*parser)->next;
}

void	free_token(t_token *token)
{
	if (!token)
		return ;
	if (token->str_data)
		free(token->str_data);
	free(token);
}

// frees a parser list that is not cirular, DOES NOT free the data var
void	clean_ncircular_parser(t_parser *parser, bool free_tok)
{
	t_parser	*last;

	while (parser)
	{
		clean_ncircular_parser(parser->arg, true);
		clean_ncircular_parser(parser->rest_name, true);
		last = parser;
		parser = parser->next;
		if (free_tok)
			free_token(last->token);
		free(last);
	}
}

/*
	- returns 1 on success, unless:
	- returns 0 if the list is empty (afterwards/before) (can be changed if needed)
	- returns -2 if the list is corrupted and does not free in that case
		(does not activly check for list intagret)
	sets *node to the element before the removed node
*/
int	remove_parser_node(t_parser **node, bool free_tok)
{
	t_parser	*last;

	if (!node)
		return (-2);
	if (!(*node))
		return (0);
	if (free_tok)
		free_token((*node)->token);
	last = (*node);
	while (last->next != (*node))
		last = last->next;
	last->next = (*node)->next;
	free(*node);
	if (last == (*node))
	{
		(*node) = NULL;
		return (0);
	}
	(*node) = last;
	return (1);
}

void	free_parser_main(t_parser *parser)
{
	while (parser)
	{
		clean_ncircular_parser(parser->arg, true);
		clean_ncircular_parser(parser->rest_name, true);
		remove_parser_node(&parser, true);
	}
}

// only allows changes to type and str_data
void	update_parser_node(t_parser *parser, t_type new_type, char *new_str_data)
{
	parser->token->type = new_type;
	if (parser->token->str_data)
		free(parser->token->str_data);
	parser->token->str_data = new_str_data;
	parser->p_type = new_type;
}
