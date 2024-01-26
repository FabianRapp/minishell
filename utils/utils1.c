/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 08:07:27 by frapp             #+#    #+#             */
/*   Updated: 2024/01/26 02:00:35 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	cleanup()
{
	printf("clean up placeholder\n");
}

bool	is_termination_char(char c)
{
	if (c == 0 || c == '(' || c == ')' || c == '|' || c == '\'' || c == '\"' 
		|| c == '>' || c == '<' || c == '*' || c == '?'
		|| c == '$' || c == '&'
		|| ft_iswhitespace(c))
	{
		return (true);
	}
	return (false);
}


// mb usefull later, for parser not useable
bool	valid_path_syntax(t_token *token)
{
	int	len;

	if (!token->str_data)
		return (0);
	len = ft_strlen(token->str_data);
	if (!token->str_data || *(token->str_data) == 0 || len > PATH_MAX)
		return (false);
	// check max individual file name length:
	while (len && (token->str_data)[len - 1] != '/')
		len--;
	if (ft_strlen(token->str_data) - len > NAME_MAX)
		return (false);
	return (true);
}

