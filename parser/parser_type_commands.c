/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_type_commands.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 19:47:45 by frapp             #+#    #+#             */
/*   Updated: 2024/02/14 06:44:41 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"
#include "internals_parser.h"
#include "../headers/lexer.h"

// abstraction for type_commands
static t_parser	*handle_operator(t_parser *parser, bool *found_command, bool *found_redir)
{
	if (!is_operator(parser->p_type))
		return (parser);
	if (!*found_command && !*found_redir)
		return (print_error(true, "handle_operator", "debug handle_operator", type_to_str(parser->p_type)), NULL);
	if (!*found_command)
	{ 
		if (insert_dummy_here(parser) == ERROR)
			return (NULL);
		parser = parser->next;
	}
	*found_command = false;
	*found_redir = false;
	parser = parser->next;
	return (parser);
}

// abstraction for type_commands()
// return value is just for readability and not used
static void	type_command(t_parser *parser, bool *found_command)
{
	if (is_operator(parser->p_type) || *found_command || parser->p_type == WHITE_SPACE
		|| is_redir(parser->p_type))
	{
		return ;
	}
	parser->p_type = COMMAND;
	*found_command = true;
}

// for type_commands()
t_result	handle_end(t_parser *parser, bool found_command, bool found_redir)
{
	if (!found_redir && !found_command)
		return (print_error(true, "DEBUG: handle_end", NULL, type_to_str(T_EOF)), ERROR);
	if (!found_command && insert_dummy_here(parser) == ERROR)
		return (ERROR);
	return (SUCCESS);
}

t_result	type_commands(t_parser *parser)
{
	bool		found_command;
	bool		found_redir;
	t_parser	*temp;

	found_command = false;
	found_redir = false;
	while (parser->p_type != T_EOF)
	{
		if (!found_redir)
			found_redir = is_redir(parser->p_type);
		if (is_operator(parser->p_type))
		{
			temp = handle_operator(parser, &found_command, &found_redir);
			if (temp == NULL)
				return (ERROR);
			parser = temp;
		}
		//else
			type_command(parser, &found_command);
		parser = parser->next;
	}
	return (handle_end(parser, found_command, found_redir));
}
