/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_nodes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 22:38:06 by frapp             #+#    #+#             */
/*   Updated: 2024/02/10 22:45:09 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/parser.h"
#include "../../headers/lexer.h"
#include "../internals_parser.h"

/*
	moves the next nodes of the parser list to the argument list of the given node
*/
void	move_to_arg(t_parser *parser, bool skip_first_whitespace,
			bool is_terminator(t_type), t_type new_type)
{
	int			len;
	t_parser	*node;
	t_parser	*arg_data;
	t_type		type;

	node = parser;
	arg_data = parser->arg;
	len = 0;
	parser = parser->next;
	type = parser->p_type;
	while (skip_first_whitespace && parser->p_type == WHITE_SPACE)
	{
		remove_parser_node(&parser, true);
		parser = parser->next;
	}
	while (arg_data && arg_data->next && arg_data->p_type != T_EOF)
		arg_data = arg_data->next;
	while (!is_terminator(parser->p_type))
	{
		parser->p_type = new_type;
		//parser->p_type = type;
		if (arg_data)
		{
			arg_data->next = parser;
			arg_data = arg_data->next;
		}
		else
		{
			arg_data = parser;
			node->arg = arg_data;
		}
		parser = parser->next;
	}
	if (arg_data)
	{
		node->next = arg_data->next;
		arg_data->next = NULL;
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
bool	move_commands_infront(t_parser *parser)
{
	t_parser	*last;

	while (parser->p_type != T_EOF)
	{
		last = last_parser(parser);
		while (parser->p_type == COMMAND && !is_operator(last->p_type) && last->p_type != T_EOF)
		{
			//TODO: figure out the diffrent syntax errors here (example: echo >aaa <sadad (echo) | >a <ad)
			if (!is_redir(last->p_type))
				return (print_error(true, NULL, NULL, type_to_str(parser->token->type)), false);
			swap_parsers(parser, last);
			last = last_parser(parser);
		}
		parser = parser->next;
	}
	return (true);
}
