/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_nodes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 22:38:06 by frapp             #+#    #+#             */
/*   Updated: 2024/02/12 18:07:58 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/parser.h"
#include "../../headers/lexer.h"
#include "../internals_parser.h"

// returns the parser node after the removed whitespace
t_parser	*remove_next_whitespaces(t_parser *parser)
{
	if (!parser)
		return (NULL);
	while (parser->p_type == WHITE_SPACE)
	{
		remove_parser_node(&parser, true);
		parser = parser->next;
	}
	return (parser);
}

void	move_to_arg(t_parser *parser, bool is_terminator(t_type), t_type new_type)
{
	t_parser	*node;
	t_parser	**next_arg;

	node = parser;
	next_arg = &(node->arg);
	parser = remove_next_whitespaces(parser->next);
	while (*next_arg)
	{
		*next_arg = (*next_arg)->next;
	}
	while (!is_terminator(parser->p_type))
	{
		parser->p_type = new_type;
		*next_arg = parser;
		parser = parser->next;
	}
	if (*next_arg)
	{
		node->next = (*next_arg)->next;
		(*next_arg)->next = NULL;
	}
}

void	swap_parsers(t_parser *node1, t_parser *node2)
{
	t_parser	*old1_next;
	t_parser	*old2_next;
	t_parser	*old1_last;
	t_parser	*old2_last;
	t_parser	*temp;

	old1_next = node1->next;
	old2_next = node2->next;
	temp = node1;
	jump_to_start(&temp);
	old1_last = temp;
	old2_last = temp;
	while (old1_last->next != node1)
		old1_last = old1_last->next;
	while (old2_last->next != node2)
		old2_last = old2_last->next;
	old1_last->next = node2;
	old2_last->next = node1;
	temp = node1->next;
	node1->next = node2->next;
	node2->next = temp;
}

// incase of leading redirs infront of command moves the comant infront of them
// otherwise this situation is bugged duo to parsing order (change needs huge refactor)
t_result	move_commands_infront(t_parser *parser)
{
	t_parser	*last;

	while (parser->p_type != T_EOF)
	{
		last = last_parser(parser);
		while (parser->p_type == COMMAND && !is_operator(last->p_type) && last->p_type != T_EOF)
		{
			//TODO: figure out the diffrent syntax errors here (example: echo >aaa <sadad (echo) | >a <ad)
			if (!is_redir(last->p_type))
			{
				print_error(true, NULL, NULL, type_to_str(parser->token->type));
				return (FAIL_ERROR);
			}
			swap_parsers(parser, last);
			last = last_parser(parser);
		}
		parser = parser->next;
	}
	return (SUCCESS);
}

// util for merge_names
void	move_next_to_name(t_parser *parser, t_parser **rest_name)
{
	if (!*rest_name)
	{
		parser->rest_name = parser->next;
		*rest_name = parser->rest_name;
	}
	else
	{
		(*rest_name)->next = parser->next;
		*rest_name = (*rest_name)->next;
	}
	parser->next = parser->next->next;
	(*rest_name)->next = NULL;
}
