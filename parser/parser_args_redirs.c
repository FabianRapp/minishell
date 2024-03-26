/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_args_redirs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 05:08:38 by frapp             #+#    #+#             */
/*   Updated: 2024/03/26 22:35:49 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

// syntax check for parse_redir_paths()
// if returns false execUtion of the current input ends
static t_result	redirs_have_arg(t_parser *parser)
{
	while (parser->p_type != T_EOF)
	{
		if (is_redir(parser->p_type) && parser->arg == NULL
			&& parser->p_type != HERE_DOC)
		{
			while (parser->next->p_type == WHITE_SPACE)
				parser = parser->next;
			print_error(true, NULL, NULL,
				type_to_error(parser->next->token->type));
			return (ERROR);
		}
		parser = parser->next;
	}
	return (SUCCESS);
}

t_result	parse_redir_paths(t_parser *parser)
{
	while (parser->p_type != T_EOF)
	{
		if (is_redir(parser->p_type) && parser->p_type != HERE_DOC)
		{
			move_to_arg(parser, is_redir_arg_terminator, REDIR_ARG, false);
		}
		parser = parser->next;
	}
	parser = parser->next;
	return (redirs_have_arg(parser));
}

void	type_args(t_parser *parser)
{
	while (parser->p_type != T_EOF)
	{
		if (parser->p_type == COMMAND)
		{
			move_to_arg(parser, command_terminator, ARGUMENT, true);
		}
		parser = parser->next;
	}
}
