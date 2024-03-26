/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:46:56 by frapp             #+#    #+#             */
/*   Updated: 2024/03/26 22:35:16 by mevangel         ###   ########.fr       */
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
	t_lexer	last;
	t_token	*token;
	char	*tmp;

	read_char(lexer);
	last = *lexer;
	token = next_new_token(lexer, false);
	while (token && (token->type == WHITE_SPACE || token->type == VOID))
	{
		free_token(token);
		last = *lexer;
		token = next_new_token(lexer, false);
	}
	if (!token)
		last.str = NULL;
	else
		free_token(token);
	tmp = lexer->str;
	*lexer = last;
	lexer->str = tmp;
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
