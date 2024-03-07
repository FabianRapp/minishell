/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_type_commands.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 19:47:45 by frapp             #+#    #+#             */
/*   Updated: 2024/03/07 08:50:00 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"
#include "internals_parser.h"
#include "../headers/lexer.h"

// for type_commands
static t_parser	*handle_operator(t_parser *parser, bool *found_command, bool *found_redir, bool *found_subshell)
{
	if (!is_operator(parser->p_type))
		return (parser);
	if (!parser->next || is_operator(parser->next->p_type) || parser->next->p_type == T_EOF)
	{
		if ((!parser->next || parser->next->p_type == T_EOF) && *found_command)
		{
			set_last_exit(2);
			if (sub_shell_mode(GET_SUB_SHELL_MODE) == true)
			{
				set_last_exit(2);
				print_error(true, NULL, NULL, "syntax error near unexpected token `)'");
				return (NULL);
			}
			else
			{
				print_error(true, NULL, NULL, "syntax error: unexpected end of file");
				print_error(false, NULL, NULL, "exit");
				full_exit_status(true);
				return (NULL);
			}
		}
		if (parser->next)
			print_error(true, "debug 0987", NULL, type_to_str(parser->next->p_type));
		else
			print_error(true, NULL, NULL, type_to_str(T_EOF));
		return (NULL);
	}
	if (!*found_command && !*found_redir)
		return (print_error(true, "DEBUG12311", NULL, type_to_str(parser->p_type)), NULL);
	if (!*found_command)
	{ 
		printf("DEBUG insert dummy at operator\n");
		if (insert_dummy_here(parser) == ERROR)
			return (NULL);
		parser = parser->next;
	}
	*found_subshell = false;
	*found_command = false;
	*found_redir = false;
	return (parser);
}

// abstraction for type_commands()
// return value is just for readability and not used
static t_result	type_command(t_parser *parser, bool *found_command)
{
	if (is_operator(parser->p_type) || (*found_command && parser->p_type != SUBSHELL) || parser->p_type == WHITE_SPACE
		|| is_redir(parser->p_type))
	{
		return (SUCCESS);
	}
	parser->p_type = COMMAND;
	*found_command = true;
	return (SUCCESS);
}

// for type_commands()
t_result	handle_end(t_parser *parser, bool found_command, bool found_redir)
{
	if (!found_redir && !found_command)
	{
		return (print_error(true, NULL, NULL, type_to_str(T_EOF)), ERROR);
	}
	if (!found_command)
	{
		if (insert_dummy_here(parser) == ERROR)
			return (ERROR);
	}
	return (SUCCESS);
}

// abstraction for type_commands
static t_parser	*handle_subshell(t_parser *parser, bool *found_command, bool *found_redir, bool *found_subshell)
{
	if (parser->p_type != SUBSHELL && (parser->token && parser->token->type != SUBSHELL))
		return (parser);
	if (*found_command || *found_redir)
	{
		print_error(true, "DEBUG 2", NULL, "syntax error near unexpected token `('");
		return (NULL);
	}
	if (parser->next && !is_operator(parser->next->p_type) && parser->next->p_type != T_EOF
		&& !is_redir(parser->next->p_type))
	{
		print_error(true, NULL, NULL, type_to_str(parser->next->p_type));
		return (NULL);
	}
	*found_command = true;
	*found_subshell = true;
	*found_redir = false;
	return (parser);
}

// if found_subshell is true found_command is also true but not the other way arround
t_result	type_commands(t_parser *parser)
{
	bool		found_command;
	bool		found_redir;
	bool		found_subshell;
	t_parser	*temp;

	found_command = false;
	found_redir = false;
	found_subshell = false;
	while (parser->p_type != T_EOF)
	{
		if (!found_redir && is_redir(parser->p_type))
		{
			found_redir = true;
		}
		else if (parser->p_type == SUBSHELL || (parser->token && parser->token->type == SUBSHELL))
		{
			temp = handle_subshell(parser, &found_command, &found_redir, &found_subshell);
			if (temp == NULL)
				return (ERROR);
			parser = temp;
		}
		else if (is_operator(parser->p_type))
		{
			temp = handle_operator(parser, &found_command, &found_redir, &found_subshell);
			if (temp == NULL)
				return (ERROR);
			parser = temp;
		}
		if (type_command(parser, &found_command) == ERROR)
			return (ERROR);
		parser = parser->next;
	}
	return (handle_end(parser, found_command, found_redir));
}
