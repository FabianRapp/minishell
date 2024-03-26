/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_nodes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 22:38:06 by frapp             #+#    #+#             */
/*   Updated: 2024/03/26 22:36:09 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	move_to_arg(t_parser *parser, bool is_terminator(t_type),
	t_type new_type, bool as_must_as_possible)
{
	t_parser	*command;
	t_parser	**next_arg;

	command = parser;
	next_arg = &(command->arg);
	while (*next_arg)
	{
		next_arg = &((*next_arg)->next);
	}
	parser = parser->next;
	while (parser && !is_terminator(parser->p_type))
	{
		parser->p_type = new_type;
		*next_arg = parser;
		next_arg = &(parser->next);
		parser = parser->next;
		if (as_must_as_possible == false)
			break ;
	}
	command->next = parser;
	*next_arg = NULL;
}

static void	swap_parser_with_next(t_parser *parser)
{
	t_parser	temp;
	t_parser	*next_next;

	if (!parser || !parser->next)
		return ;
	next_next = parser->next->next;
	temp = *parser;
	*parser = *(parser->next);
	parser->next = temp.next;
	*(temp.next) = temp;
	temp.next->next = next_next;
}

// moves the command of one command block to the head of the block
// assumes exactly one commmand per block (including dummy commands)
// returns the parser node after the command block
static t_parser	*fix_command_block(t_parser *parser)
{
	t_parser	*head;

	head = parser;
	if (is_command_block_terminator(parser->p_type))
		return (parser);
	while (!is_command_block_terminator(parser->next->p_type))
	{
		if (parser->next->p_type == COMMAND)
		{
			swap_parser_with_next(parser);
			parser = head;
		}
		else
			parser = parser->next;
	}
	return (parser->next);
}

// incase of leading redirs infront of command moves the comant infront of them
// otherwise this situation is bugged duo to parsing order
void	move_commands_infront(t_parser *parser)
{
	while (parser->p_type != T_EOF)
	{
		parser = fix_command_block(parser);
		if (parser->p_type == T_EOF)
			return ;
		parser = parser->next;
	}
}

void	move_next_to_restname(t_parser *parser, t_parser **rest_name)
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
