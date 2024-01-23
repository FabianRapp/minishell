/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   groups1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 20:34:43 by frapp             #+#    #+#             */
/*   Updated: 2024/01/23 20:41:24 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"
#include "internals_parser.h"
#include "../headers/lexer.h"

bool	is_redir(t_type type)
{
	if (type == REDIR_IN
		|| type == REDIR_OUT
		|| type == REDIR_APPEND
		|| type == HERE_DOC)
	{
		return (true);
	}
	return (false);
}

bool	is_redir_arg_terminator(t_type type)
{
	if (type != T_EOF && type != OR && type != AND && type != PIPE
		&& !is_redir(type) && type != WHITE_SPACE)
	{
		return (false);
	}
	return (true);
}

bool	is_operator(t_type type)
{
	if (type == PIPE || type == OR || type == AND)
	{
		return (true);
	}
	return (false);
}
