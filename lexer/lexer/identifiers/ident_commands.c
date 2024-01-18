/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ident_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:36:26 by frapp             #+#    #+#             */
/*   Updated: 2024/01/18 13:40:56 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/lexer.h"
#include "../internals.h"


void	ident_commands_type(t_token *list)
{
	bool	next_word_command;
	t_type	type;

	next_word_command = true;
	while (list->type != T_EOF)
	{
		skip_whitespace(&list);
		type = list->type;
		if (next_word_command && type == WORD)
		{
			type = COMMAND;
			next_word_command = false;
		}
		else if (!next_word_command && type == WORD)
		{
			type = ARGUMENT;
		}
		else if (type == FLAG)
			continue ;
		else if (type == UNKNOWN)
			printf("error in ident_commands_type(): unknown token: %c\n", list->unknown);
		else if (type == PIPE || type == OR || type == AND || type == EXIT_STATUS_REQUEST || )
	}
}
