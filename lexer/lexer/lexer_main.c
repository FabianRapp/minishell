/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:42:58 by frapp             #+#    #+#             */
/*   Updated: 2024/01/30 03:45:03 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/lexer.h"
#include "internals.h"

/*
	for cleanup:
	token.str has to be checked for NULL
	if not NULL "free(token.str)" has to be called
*/

t_token	*lexer_malloc_fail(t_token *token, bool *malloc_error)
{
	my_free((void **)&(token->str_data));
	my_free((void **)&(token->old_data));
	my_free((void **)&(token));
	*malloc_error = true;
	return (NULL);
}

t_token	*next_new_token(t_lexer *lexer, bool *malloc_error)
{
	t_token		*token;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return(lexer_malloc_fail(token, malloc_error));
	init_token(token, lexer);
	if ((*malloc_error) || basic_sign_type(lexer, token))
	{
	}
	else if ((*malloc_error) || literal_type(lexer, token, malloc_error))
	{
		if (*malloc_error)
			return(lexer_malloc_fail(token, malloc_error));
	}
	else if ((*malloc_error) || interpreted_type(lexer, token, malloc_error))
	{
		if (*malloc_error)
			return(lexer_malloc_fail(token, malloc_error));
	}
	else if ((*malloc_error) || redir_type(lexer, token))
	{
	}
	else if ((*malloc_error) || env_var_type(lexer, token, malloc_error))
	{
		if (*malloc_error)
			return(lexer_malloc_fail(token, malloc_error));
	}
	else if ((*malloc_error) || subshell_type(lexer, token, malloc_error))
	{
		if (*malloc_error)
			return(lexer_malloc_fail(token, malloc_error));
	}
	else if ((*malloc_error) || literal_type2(lexer, token, malloc_error))
	{
		if (*malloc_error)
			return(lexer_malloc_fail(token, malloc_error));
	}
	else
	{
		printf("DEBUG: no function IDed the type\n");
		exit (1);
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

	while (str && *str && ft_iswhitespace(*str))
		str++;
	lexer.position = 0;
	lexer.read_position = 0;
	lexer.str = str;
	read_char(&lexer);
	return (lexer);
}
