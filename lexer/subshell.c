/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 21:34:29 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 04:36:30 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static t_result	verify_subshell_chars(t_lexer *lexer)
{
	int		count_open;

	if ((lexer->str)[lexer->read_position] == ')')
	{
		print_error(true, NULL,
			NULL, "syntax error near unexpected token `)\'");
		return (set_last_exit(2), ERROR);
	}
	count_open = 1;
	while ((lexer->str)[lexer->read_position] && count_open)
	{
		if ((lexer->str)[lexer->read_position] == '(')
			count_open++;
		else if ((lexer->str)[lexer->read_position] == ')')
			count_open--;
		(lexer->read_position)++;
	}
	//if (count_open && !(bool)TESTER)
		//full_exit_status(true);
	if (count_open)
		return (print_error(true, NULL, "syntax error",
				" unexpected end of file\nexit\n"), set_last_exit(2), ERROR);
	return (SUCCESS);
}

t_result	subshell_type(t_lexer *lexer, t_token *token)
{
	if (lexer->cur_char != '(')
		return (SUCCESS);
	if (verify_subshell_chars(lexer) == ERROR)
		return (ERROR);
	token->type = SUBSHELL;
	token->str_data = ft_strndup(lexer->str + lexer->position + 1,
			lexer->read_position - lexer->position - 2);
	if (!token->str_data)
		return (set_last_exit(errno), ERROR);
	read_char(lexer);
	return (SUCCESS);
}
