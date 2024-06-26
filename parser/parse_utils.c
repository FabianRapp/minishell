/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 16:16:15 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/27 17:43:52 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

t_result	has_content(t_parser *parser)
{
	t_parser	*head;

	if (!parser)
		return (ERROR);
	head = parser;
	while (parser->p_type == WHITE_SPACE || parser->p_type == VOID
		|| parser->p_type == T_EOF)
	{
		parser = parser->next;
		if (parser == head)
		{
			free_parser_main(parser, true);
			return (ERROR);
		}
	}
	return (SUCCESS);
}

// might not be needed
t_parser	*has_none_redir_arg(t_parser *parser)
{
	t_parser	*cur_arg;

	cur_arg = parser->arg;
	while (cur_arg)
	{
		if (!(is_redir(cur_arg->p_type) || is_redir(cur_arg->token->type)))
			return (cur_arg);
		cur_arg = cur_arg->next;
	}
	return (NULL);
}

// abstraction for type_commands()
void	type_command(t_parser *parser, bool *found_command)
{
	if (is_operator(parser->p_type)
		|| (*found_command && parser->p_type != SUBSHELL)
		|| parser->p_type == WHITE_SPACE
		|| is_redir(parser->p_type))
	{
		return ;
	}
	parser->p_type = COMMAND;
	*found_command = true;
}
