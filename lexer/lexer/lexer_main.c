/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:42:58 by frapp             #+#    #+#             */
/*   Updated: 2024/01/23 21:16:23 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/lexer.h"
#include "internals.h"

/*
	for cleanup:
	token.str has to be checked for NULL
	if not NULL "free(token.str)" has to be called
*/
t_token	*next_new_token(t_lexer *lexer)
{
	t_token		*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return(cleanup(), NULL);
	init_token(token, lexer);
	if (basic_sign_type(lexer, token))
	{
		if (DEBUG) printf("basic_sign_type\n");
	}
	else if (literal_type(lexer, token))
	{
		if (DEBUG) printf("literal_type\n");
	}
	else if (interpreted_type(lexer, token))
	{
		if (DEBUG) printf("interpreted_type\n");
	}
	else if (ft_buildin_type(lexer, token))
	{
		if (DEBUG) printf("ft_buildin_type\n");
	}
	else if (redir_type(lexer, token))
	{
		if (DEBUG) printf("redir_type\n");
	}
	else if (env_var_type(lexer, token))
	{
		if (DEBUG) printf("env_var_type\n");
	}
	else if (subshell_type(lexer, token))
	{
		if (DEBUG) printf("subshell_type\n");
	}
	// else if (integer_type(lexer, token))
	// {
	// 	if (DEBUG) printf("integer_type\n");
	// }
	// else if (flag_type(lexer, token))
	// {
	// 	if (DEBUG) printf("flag_type\n");
	// }
	else if (word_type(lexer, token))
	{
		if (DEBUG) printf("word_type\n");
	}
	else if (DEBUG)
	{
		printf("no function IDed the type\n");
	}
	if (token->type == UNKNOWN)
		token->unknown = lexer->cur_char;
	read_char(lexer);
	return (token);
}

// inits a lexer object, returns the object
// NOT a pointer to a dynamic memory location!!
// takes a NULL-terminated str
t_lexer	new_lexer(char *str)
{
	t_lexer		lexer;

	lexer.position = 0;
	lexer.read_position = 0;
	lexer.str = str;
	read_char(&lexer);
	return (lexer);
}



