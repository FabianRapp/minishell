/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 09:26:13 by frapp             #+#    #+#             */
/*   Updated: 2024/01/30 03:39:45 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parser.h"
#include "../headers/lexer.h"
#include "internals_parser.h"

bool	insert_token(t_parser **parser, t_token *token, bool *malloc_error)
{
	t_parser	*next;

	if (!token || !parser)
	{
		printf ("error insert_token\n");
		return (false);
	}
	if (!*parser)
		*parser = ft_calloc(1, sizeof(t_parser));
	if (!*parser)
	{
		*malloc_error = true;
		return (false);
	}
	if (!(*parser)->token)
	{
		if ((*parser)->next)
		{
			printf("error in insert_token\n");
			return (false);
		}
		(*parser)->p_type = token->type;
		(*parser)->token = token;
		(*parser)->next = (*parser);
		return (true);
	}
	next = (*parser)->next;
	(*parser)->next = ft_calloc(1, sizeof(t_parser));
	if (!(*parser)->next)
	{
		*malloc_error = true;
		return (false);
	}
	(*parser)->next->token = token;
	(*parser)->next->p_type = token->type;
	(*parser) = (*parser)->next;
	(*parser)->next = next;;
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
	bool			malloc_error;
	t_parser		*head;

	malloc_error = false;
	first = NULL;
	token = NULL;
	parser = NULL;
	lexer = new_lexer(str);
	if (!lexer.str || lexer.cur_char == 0)
		return (NULL);
	while (!token || token->type != T_EOF)
	{
		token = next_new_token(&lexer, &malloc_error);
		if (malloc_error)
		{
			free_parser_main(parser, true);
			return (NULL);
		}
		if (token->type == VOID)
		{
			free_token(token);
			continue ;
		}
		//for malloc_fail
		if (parser)
			head = parser->next;
		else
			head = NULL;
		if (!insert_token(&parser, token, &malloc_error))
		{
			if (malloc_error && head)
			{
				parser->next = head;
				free_parser_main(parser, true);
			}
			return (NULL);
		}
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
	my_free((void **)&(token->str_data));
	my_free((void **)&(token->old_data));
	free(token);
}

// frees a parser list that is not cirular, DOES NOT free the data var
void	free_ncircular_parser(t_parser *parser, bool free_tok)
{
	t_parser	*last;

	while (parser)
	{
		free_ncircular_parser(parser->arg, free_tok);
		free_ncircular_parser(parser->rest_name, free_tok);
		last = parser;
		parser = parser->next;
		if (free_tok || last->token->type == T_EOF || is_redir(last->token->type))
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
	if (free_tok || (*node)->token->type == T_EOF || is_redir((*node)->token->type))
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

void	free_parser_main(t_parser *parser, bool free_tokens)
{
	while (parser)
	{
		free_ncircular_parser(parser->arg, free_tokens);
		free_ncircular_parser(parser->rest_name, free_tokens);
		remove_parser_node(&parser, free_tokens);
	}
}
