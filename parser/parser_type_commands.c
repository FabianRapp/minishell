/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_type_commands.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 19:47:45 by frapp             #+#    #+#             */
/*   Updated: 2024/02/12 20:06:01 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"
#include "internals_parser.h"
#include "../headers/lexer.h"

// abstraction for type_commands
static t_result	handle_operator(t_parser *parser, bool *found_command, bool *found_redir)
{
	if (!is_operator(parser->p_type))
		return (SUCCESS);
	if (!*found_command && !*found_redir && is_operator(parser->p_type))
		return (print_error(true, NULL, NULL, type_to_str(parser->p_type)), ERROR);
	if (!*found_command && *found_redir && insert_dummy(parser) == ERROR)
		return (ERROR);
	*found_command = false;
	*found_redir = false;
	return (SUCCESS);
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
static t_result	handle_end(t_parser *parser, bool found_command, bool found_redir)
{
	if (!found_redir && !found_command)
		return (print_error(true, NULL, NULL, type_to_str(T_EOF)), ERROR);
	if (found_redir && !found_command && insert_dummy(parser) == ERROR)
		return (ERROR);
	return (SUCCESS);
}

t_result	type_commands(t_parser *parser)
{
	bool		found_command;
	bool		found_redir;

	found_command = false;
	found_redir = false;
	while (parser->p_type != T_EOF)
	{
		found_redir = is_redir(parser->p_type);
		if (handle_operator(parser, &found_command, &found_redir) == ERROR)
			return (ERROR);
		type_command(parser, &found_command);
		parser = parser->next;
	}
	return (handle_end(parser, found_command, found_command));
}

// bool	handle_operator(t_parser *parser, bool found_command, bool found_redir)
// {
// 	if (!found_command && !found_redir && is_operator(parser->p_type))
// 		return (print_error(true, NULL, NULL, type_to_str(parser->p_type)), false);
// 	if (!found_command && found_redir && !insert_dummy(parser))
// 		return (false);
// 	return (true);
// }

// t_result	type_commands(t_parser *parser)
// {
// 	bool		found_command;
// 	bool		found_redir;

// 	found_command = false;
// 	found_redir = false;
// 	while (parser->p_type != T_EOF)
// 	{
// 		found_redir = is_redir(parser->p_type);
// 		if (is_operator(parser->p_type))
// 		{
// 			if (!handle_operator(parser, found_command, found_redir))
// 				return (false);
// 			found_command = false;
// 			found_redir = false;
// 		}
// 		else if (!found_command && !is_operator(parser->p_type) && parser->p_type != WHITE_SPACE
// 				&& !is_redir(parser->p_type))
// 		{
// 			parser->p_type = COMMAND;
// 			found_command = true;
// 		}
// 		parser = parser->next;
// 	}
// 	if (!found_redir && !found_command)
// 	{
// 		print_error(true, NULL, NULL, type_to_str(T_EOF));
// 		return (false);
// 	}
// 	if (found_redir && !found_command && !insert_dummy(parser))
// 		return (false);
// 	return (true);
// }