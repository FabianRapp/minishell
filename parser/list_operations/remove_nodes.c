/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_nodes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 22:36:42 by frapp             #+#    #+#             */
/*   Updated: 2024/03/06 00:21:51 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/parser.h"
#include "../../headers/lexer.h"
#include "../internals_parser.h"

void	trim_whitespace(t_parser *parser)
{
	bool	was_whitespace;

	was_whitespace = false;
	while (parser->p_type != T_EOF)
	{
		if (parser->p_type == WHITE_SPACE)
		{
			if (was_whitespace)
				remove_parser_node(&parser, true);
			was_whitespace = true;
		}
		else
			was_whitespace = false;
		parser = parser->next;
	}
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
		if (free_tok || last->token->type == T_EOF
			|| is_redir(last->token->type))
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

void	remove_whitespace(t_parser *parser)
{
	while (parser->p_type != T_EOF)
	{
		if (parser->p_type == WHITE_SPACE)
		{
			remove_parser_node(&parser, true);
		}
		parser = parser->next;
	}
}
