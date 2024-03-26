/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_to_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 03:35:59 by frapp             #+#    #+#             */
/*   Updated: 2024/03/26 22:38:04 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static char	*type_to_error_2(t_type type)
{
	if (type == REDIR_IN)
		return ("syntax error near unexpected token `>'");
	if (type == REDIR_APPEND)
		return ("syntax error near unexpected token `>>'");
	if (type == HERE_DOC)
		return ("syntax error near unexpected token `<<'");
	if (type == HERE_STR)
		return ("syntax error near unexpected token `<<<'");
	if (type == SUBSHELL)
		return ("syntax error near unexpected token `(");
	if (type == COMMAND || type == ARGUMENT || type == REDIR_ARG)
		return ("syntax error near unexpected word token");
	if (type == VOID)
		return ("unexpected void type");
	if (type == DUMMY_COMMAND)
		return (NULL);
	if (type == LITERAL)
		return ("syntax error near unexpected token `''");
	if (type == PID_REQUEST)
		return ("syntax error near unexpected token `$$'");
	if (type == SEMICOL)
		return ("syntax error near unexpected token `;'");
	return (NULL);
}

static char	*type_to_error_1(t_type type)
{
	if (type == UNKNOWN)
		return ("UNKNOWN");
	if (type == T_EOF)
		return ("syntax error: unexpected end of file");
	if (type == WHITE_SPACE)
		return (" ");
	if (type == PIPE)
		return ("syntax error near unexpected token `|'");
	if (type == OR)
		return ("syntax error near unexpected token `||'");
	if (type == AND)
		return ("syntax error near unexpected token `&&'");
	if (type == ENV_VAR || type == EXIT_STATUS_REQUEST)
		return ("syntax error near unexpected token `$'");
	if (type == WILDCARD)
		return ("syntax error near unexpected token `*'");
	if (type == WORD)
		return ("syntax error near unexpected word token");
	if (type == INTERPRETED)
		return ("syntax error near unexpected token `\"'");
	return (type_to_error_2(type));
}

char	*type_to_error(t_type type)
{
	return (type_to_error_1(type));
}
