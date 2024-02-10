/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 22:13:59 by frapp             #+#    #+#             */
/*   Updated: 2024/02/10 22:20:43 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/parser.h"
#include "../../headers/lexer.h"
#include "../internals_parser.h"

// creates a cricular singular linked list
// each node has to be freed and each tokens values as a normal token would
t_parser	*init_parser(char *str)
{
	t_token			*token;
	t_parser		*first;
	t_lexer			lexer;
	t_parser		*parser;

	first = NULL;
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
		first = insert_token(&parser, token);
		if (!first)
			return (NULL);
	}
	return (first);
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

void	free_parser_main(t_parser *parser, bool free_tokens)
{
	while (parser)
	{
		free_ncircular_parser(parser->arg, free_tokens);
		free_ncircular_parser(parser->rest_name, free_tokens);
		remove_parser_node(&parser, free_tokens);
	}
}
