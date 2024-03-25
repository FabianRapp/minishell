/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   groups2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 00:15:26 by frapp             #+#    #+#             */
/*   Updated: 2024/03/25 00:37:08 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

bool	is_termination_char(char c)
{
	if (c == 0
		|| c == '('
		|| c == ')'
		|| c == '|'
		|| c == '\''
		|| c == '\"'
		|| c == '>'
		|| c == '<'
		|| c == '$'
		|| c == '&'
		|| ft_iswhitespace(c)
		|| c == ';'
	)
	{
		return (true);
	}
	return (false);
}

bool	is_wildcard_block_termination(char c)
{
	if (c == 0
		|| c == '('
		|| c == ')'
		|| c == '|'
		|| c == '>'
		|| c == '<'
		|| c == '$'
		|| c == '&'
		|| ft_iswhitespace(c)
		|| c == ';'
	)
	{
		return (true);
	}
	return (false);
}

bool	is_operator(t_type type)
{
	if (type == PIPE
		|| type == OR
		|| type == AND
		|| type == SEMICOL
	)
	{
		return (true);
	}
	return (false);
}
