/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_ident_literal_wildcards.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 01:13:29 by frapp             #+#    #+#             */
/*   Updated: 2024/03/26 22:35:04 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static bool	contains_more_wildcards(char *str)
{
	bool	except;

	except = false;
	str++;
	while (!is_wildcard_block_termination(*str) || except)
	{
		if (*str == '\\')
			except = true;
		if (!except && *str == '*')
			return (true);
		str++;
	}
	return (false);
}

static bool	contained_a_wildcard(t_lexer lexer)
{
	lexer.position -= 1;
	while (lexer.position >= 0
		&& (!is_wildcard_block_termination(lexer.str[lexer.position])
			&& lexer.position - 1 >= 0
			&& lexer.str[lexer.position - 1] != '\\'))
	{
		if (lexer.str[lexer.position] == '*' && lexer.position - 1 >= 0
			&& lexer.str[lexer.position - 1] != '\\')
			return (true);
		lexer.position -= 1;
	}
	return (false);
}

static t_result	handle_wildcard(t_lexer *lexer, bool is_start, t_token *token)
{
	char	*tmp;

	if (!is_start && !contained_a_wildcard(*lexer))
	{
		if (!ft_strjoin_inplace(&(token->str_data), "1}{"))
			return (ERROR);
		if (!contains_more_wildcards(lexer->str + lexer->position))
		{
			tmp = ft_strndup(lexer->str, lexer->position + 1);
			if (!ft_strjoin_inplace(&tmp, "3}{!") || !ft_strjoin_inplace(&tmp,
					lexer->str + lexer->position + 1))
				return (ERROR);
			free(lexer->str);
			lexer->str = tmp;
		}
	}
	else if (contains_more_wildcards(lexer->str + lexer->position))
	{
		if (!ft_strjoin_inplace(&(token->str_data), "2}{"))
			return (ERROR);
	}
	else
		if (!ft_strjoin_inplace(&(token->str_data), "3}{"))
			return (ERROR);
	return (SUCCESS);
}

static t_result	copy_str(t_lexer *lexer,
	t_token *token, bool skip_next_term, bool is_start)
{
	while (lexer->cur_char && ((!is_termination_char(lexer->cur_char)
				|| skip_next_term)))
	{
		if (!skip_next_term && lexer->cur_char == '*'
			&& handle_wildcard(lexer, is_start, token) == ERROR)
			return (ERROR);
		if (!skip_next_term && lexer->cur_char == '\\')
		{
			skip_next_term = true;
			read_char(lexer);
			continue ;
		}
		skip_next_term = false;
		if (!ft_strjoin_inplace_char(&(token->str_data), lexer->cur_char))
			return (ERROR);
		read_char(lexer);
		is_start = false;
	}
	return (SUCCESS);
}

t_result	ident_wildcard_literals(t_lexer *lexer,
	t_token *token, bool skip_next_term)
{
	bool	is_start;

	if (is_termination_char(lexer->cur_char) && !skip_next_term)
		return (SUCCESS);
	token->str_data = ft_calloc(1, 1);
	if (!token->str_data)
		return (ERROR);
	is_start = true;
	if (lexer->position && (lexer->str[lexer->position - 1] == '\''
			|| lexer->str[lexer->position - 1] == '\"')
		&& lexer->position - 1 && lexer->str[lexer->position - 2] != '\\')
		is_start = false;
	if (copy_str(lexer, token, skip_next_term, is_start) == ERROR)
		return (ERROR);
	token->type = LITERAL;
	return (SUCCESS);
}
