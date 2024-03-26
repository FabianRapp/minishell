/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   groups1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 20:34:43 by frapp             #+#    #+#             */
/*   Updated: 2024/03/26 22:37:50 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

bool	is_command_block_terminator(t_type type)
{
	if (is_operator(type) || type == T_EOF)
		return (true);
	return (false);
}

bool	is_redir(t_type type)
{
	if (type == REDIR_IN
		|| type == REDIR_OUT
		|| type == REDIR_APPEND
		|| type == HERE_DOC
		|| type == REDIR_IN_OUT
		|| type == HERE_STR
	)
	{
		return (true);
	}
	return (false);
}

bool	is_redir_arg_terminator(t_type type)
{
	if (type == T_EOF
		|| type == OR
		|| type == AND
		|| type == PIPE
		|| type == SEMICOL
		|| is_redir(type)
		|| type == WHITE_SPACE
	)
	{
		return (true);
	}
	return (false);
}

bool	is_word_terminator(t_type type)
{
	if (type != T_EOF && type != OR && type != AND && type != PIPE
		&& !is_redir(type) && type != WHITE_SPACE && type != SUBSHELL
		&& type != COMMAND && type != DUMMY_COMMAND
		&& type != SEMICOL
	)
	{
		return (false);
	}
	return (true);
}

bool	command_terminator(t_type type)
{
	if (type == PIPE
		|| type == OR
		|| type == AND
		|| type == WHITE_SPACE
		|| type == T_EOF
		|| type == SUBSHELL
		|| type == COMMAND
		|| type == DUMMY_COMMAND
		|| type == SEMICOL
	)
	{
		return (true);
	}
	return (false);
}
