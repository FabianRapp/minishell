/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:46:56 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 05:49:42 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

// reads the next char into the lexer and updates indexes
void	read_char(t_lexer *lexer)
{
	if (lexer->read_position > ((int)ft_strlen(lexer->str)))
	{
		printf("debug read_char\n");
		exit(1);
	}
	lexer->last_char = lexer->cur_char;
	if (lexer->str)
		lexer->cur_char = (lexer->str)[lexer->read_position];
	lexer->position = lexer->read_position;
	if (lexer->read_position < ((int)ft_strlen(lexer->str)))
		(lexer->read_position)++;
	else
		lexer->read_position = (int)ft_strlen(lexer->str);
}

bool	is_redir_terminator_char(char c)
{
	if (ft_iswhitespace(c) || c == '|' || c == '&' || c == '('
		|| c == ')' || c == '\0' || c == '<' || c == '>'
		|| c == ';')
	{
		return (true);
	}
	return (false);
}

void	lexer_error(t_token *token)
{
	if (token)
	{
		ft_free((void **)&(token->str_data));
		ft_free((void **)&(token->old_data));
		ft_free((void **)&(token->left_redir_arg));
		ft_free((void **)&(token));
	}
}

// util for new_lexer
static void	skip_leading_void_whitespace(t_lexer *lexer)
{
	read_char(lexer);
	while (lexer->cur_char && ft_iswhitespace(lexer->cur_char))
	{
		read_char(lexer);
	}
	if (!lexer->cur_char)
	{
		free(lexer->str);
		lexer->str = NULL;
	}
}

// inits a lexer object, returns the object
// NOT a pointer to a dynamic memory location!
t_lexer	new_lexer(char *str)
{
	t_lexer		lexer;

	lexer.cur_char = 0;
	lexer.last_char = 0;
	lexer.position = 0;
	lexer.read_position = 0;
	lexer.str = NULL;
	lexer.str = ft_strdup(str);
	if (!lexer.str)
		return (lexer);
	skip_leading_void_whitespace(&lexer);
	return (lexer);
}
