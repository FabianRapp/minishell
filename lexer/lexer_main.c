/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:42:58 by frapp             #+#    #+#             */
/*   Updated: 2024/02/10 21:32:04 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/lexer.h"
#include "internals.h"

/*
	for cleanup:
	token.str has to be checked for NULL
	if not NULL "free(token.str)" has to be called
*/
void	lexer_error(t_token *token)
{
	if (token)
	{
		my_free((void **)&(token->str_data));
		my_free((void **)&(token->old_data));
		my_free((void **)&(token));
	}
}

bool	invalid_char(t_lexer *lexer)
{
	if (lexer->cur_char == ')')
	{
		print_error(true, NULL, NULL, "syntax error near unexpected token `)\'");
		return (true);
	}
	return (false);
}

t_token	*classify_sub_str(t_token *token, t_lexer *lexer)
{
	if (invalid_char(lexer))
		return (lexer_error(token), NULL);
	basic_sign_type(lexer, token);
	if (!token->type && !literal_type(lexer, token))
		return (lexer_error(token), NULL);
	if (!token->type && !interpreted_type(lexer, token))
		return (lexer_error(token), NULL);
	if (!token->type && !redir_type(lexer, token))
		return (lexer_error(token), NULL);
	if (!token->type && !dollar_lexing(lexer, token))
		return (lexer_error(token), NULL);
	if (!token->type && !subshell_type(lexer, token))
		return (lexer_error(token), NULL);
	if (!token->type && !literal_type2(lexer, token))
		return (lexer_error(token), NULL);
	token->unknown = lexer->cur_char;
	return (token);
}

t_token	*next_new_token(t_lexer *lexer)
{
	t_token		*token;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL);
	init_token(token, lexer);
	if (!classify_sub_str(token, lexer))
		return (NULL);
	if (token->type)
		return (read_char(lexer), token);
	printf("DEBUG: no function IDed the type\n");
	printf("%s\n", lexer->str + lexer->position);
	return (lexer_error(token), exit(1), NULL);
}

// inits a lexer object, returns the object
// NOT a pointer to a dynamic memory location!!
// takes a NULL-terminated str
t_lexer	new_lexer(char *str)
{
	t_lexer		lexer;

	while (str && *str && ft_iswhitespace(*str))
		str++;
	lexer.position = 0;
	lexer.read_position = 0;
	lexer.str = str;
	read_char(&lexer);
	return (lexer);
}
