/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   groups1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 20:34:43 by frapp             #+#    #+#             */
/*   Updated: 2024/03/11 17:17:58 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"
#include "../headers/lexer.h"

bool	is_command_block_terminator(t_type type)
{
	if (is_operator(type) || type == T_EOF)// || type == SUBSHELL)
		return (true);
	return (false);
}

bool	is_redir(t_type type)
{
	if (type == REDIR_IN
		|| type == REDIR_OUT
		|| type == REDIR_APPEND
		|| type == HERE_DOC
		|| type == REDIR_IN_OUT)
	{
		return (true);
	}
	return (false);
}

bool	is_redir_arg_terminator(t_type type)
{
	if (type == T_EOF || type == OR || type == AND || type == PIPE
		|| is_redir(type) || type == WHITE_SPACE)
	{
		return (true);
	}
	return (false);
}

bool	is_word_terminator(t_type type)
{
	if (type != T_EOF && type != OR && type != AND && type != PIPE
		&& !is_redir(type) && type != WHITE_SPACE && type != SUBSHELL
		&& type != COMMAND && type != DUMMY_COMMAND)
	{
		return (false);
	}
	return (true);
}

bool	command_terminator(t_type type)
{
	if (type == PIPE || type == OR || type == AND || type == WHITE_SPACE || type == T_EOF
		|| type == SUBSHELL || type == COMMAND || type == DUMMY_COMMAND)
	{
		return (true);
	}
	return (false);
}
