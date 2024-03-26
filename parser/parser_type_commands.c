/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_type_commands.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 19:47:45 by frapp             #+#    #+#             */
/*   Updated: 2024/03/26 22:35:44 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static void	handle_operator_2ndscenario(t_parser *parser, bool *found_command)
{
	set_last_exit(2);
	if ((!parser->next || parser->next->p_type == T_EOF) && *found_command)
	{
		if (sub_shell_mode(GET_SUB_SHELL_MODE) == true)
			print_error(true, NULL, NULL,
				"syntax error near unexpected token `)'");
		else if (full_exit_status(true))
			print_error(true, NULL, NULL,
				"syntax error: unexpected end of file\nexit");
	}
	else if (parser->next)
		print_error(true, NULL, NULL, type_to_error(parser->next->p_type));
	else
		print_error(true, NULL, NULL, type_to_error(T_EOF));
}

// for type_commands
static t_parser	*handle_operator(t_parser *parser,
	bool *found_command, bool *found_redir, bool *found_subshell)
{
	if (!*found_command && !*found_redir)
		return (set_last_exit(2), print_error(true, NULL, NULL,
				type_to_error(parser->p_type)), NULL);
	else if (!parser->next || is_operator(parser->next->p_type)
		|| parser->next->p_type == T_EOF)
	{
		handle_operator_2ndscenario(parser, found_command);
		return (NULL);
	}
	else if (!*found_command && insert_dummy_here(parser) == ERROR)
		return (NULL);
	else if (!*found_command)
		parser = parser->next;
	*found_subshell = false;
	*found_redir = false;
	return (parser);
}

// for type_commands()
static t_result	parser_handle_end(t_parser *parser, bool found_command,
	bool found_redir)
{
	if (!found_redir && !found_command)
	{
		print_error(true, NULL, NULL, type_to_error(T_EOF));
		set_last_exit(2);
		return (ERROR);
	}
	if (!found_command)
	{
		if (insert_dummy_here(parser) == ERROR)
			return (ERROR);
	}
	return (SUCCESS);
}

// abstraction for type_commands
static t_result	handle_subshell(t_parser *parser, bool *found_command,
	bool *found_redir, bool *found_subshell)
{
	if (parser->p_type != SUBSHELL
		&& (parser->token && parser->token->type != SUBSHELL))
		return (SUCCESS);
	if (*found_command || *found_redir)
	{
		set_last_exit(2);
		print_error(true, "DEBUG 2", NULL,
			"syntax error near unexpected token `('");
		return (ERROR);
	}
	if (parser->next && !is_operator(parser->next->p_type)
		&& parser->next->p_type != T_EOF
		&& !is_redir(parser->next->p_type))
	{
		set_last_exit(2);
		print_error(true, NULL, NULL, type_to_error(parser->next->p_type));
		return (ERROR);
	}
	*found_command = true;
	*found_subshell = true;
	*found_redir = false;
	return (SUCCESS);
}

// if found_subshell is true found_command is also
// true but not the other way arround
t_result	type_commands(t_parser *parser)
{
	bool		found_command;
	bool		found_redir;
	bool		found_subshell;

	found_command = false;
	found_redir = false;
	found_subshell = false;
	while (parser->p_type != T_EOF)
	{
		found_redir = found_redir || is_redir(parser->p_type);
		if (handle_subshell(parser, &found_command,
				&found_redir, &found_subshell) == ERROR)
			return (ERROR);
		if (is_operator(parser->p_type))
		{
			parser = handle_operator(parser, &found_command,
					&found_redir, &found_subshell);
			if (parser == NULL)
				return (ERROR);
			found_command = false;
		}
		type_command(parser, &found_command);
		parser = parser->next;
	}
	return (parser_handle_end(parser, found_command, found_redir));
}
