/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:42:58 by frapp             #+#    #+#             */
/*   Updated: 2024/02/10 20:26:11 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/lexer.h"
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

t_token	*next_new_token(t_lexer *lexer)
{
	t_token		*token;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL);
	init_token(token, lexer);
	basic_sign_type(lexer, token);
	if (token->type)
		return (read_char(lexer), token);
	if (!literal_type(lexer, token))
		return (lexer_error(token), NULL);
	if (token->type)
		return (read_char(lexer), token);
	if (!interpreted_type(lexer, token))
		return (lexer_error(token), NULL);
	if (token->type)
		return (read_char(lexer), token);
	redir_type(lexer, token);
	if (token->type)
		return (read_char(lexer), token);
	if (!env_var_type(lexer, token))
		return (lexer_error(token), NULL);
	if (token->type)
		return (read_char(lexer), token);
	if (!subshell_type(lexer, token))
		return (lexer_error(token), NULL);
	if (token->type)
		return (read_char(lexer), token);
	if (!literal_type2(lexer, token))
		return (lexer_error(token), NULL);
	if (token->type)
		return (read_char(lexer), token);
	token->unknown = lexer->cur_char;
	printf("DEBUG: no function IDed the type\n");
	printf("%s\n", lexer->str + lexer->position);
	exit(1);
	return (lexer_error(token), NULL);
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
