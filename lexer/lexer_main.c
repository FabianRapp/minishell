/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:42:58 by frapp             #+#    #+#             */
/*   Updated: 2024/03/10 13:08:45 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/lexer.h"
#include "internals.h"

t_result	valid_first_char(t_lexer *lexer)
{
	if (lexer->cur_char == ')')
	{
		print_error(true, "debug valid_first_char",
			NULL, "syntax error near unexpected token `)\'");
		return (ERROR);
	}
	return (SUCCESS);
}

t_result	handle_exception_char(t_lexer *lexer, t_token *token)
{
	if (lexer->cur_char != '\\')
		return (SUCCESS);
	read_char(lexer);
	return (literal_type2(lexer, token));
}

t_token	*classify_sub_str(t_token *token, t_lexer *lexer, bool recursive_call)
{
	//printf("lexer str at start: %s\n", lexer-> str + lexer->position);
	if (handle_exception_char(lexer, token) == ERROR)
		return (lexer_error(token), NULL);
	basic_sign_type(lexer, token);
	if (!token->type && valid_first_char(lexer) == ERROR)
		return (lexer_error(token), NULL);
	else if (!token->type && literal_type(lexer, token) == ERROR)
		return (lexer_error(token), NULL);
	else if (!token->type && interpreted_type(lexer, token) == ERROR)
		return (lexer_error(token), NULL);
	else if (!token->type && redir_type(lexer, token, recursive_call) == ERROR)
		return (lexer_error(token), NULL);
	else if (!token->type && dollar_lexing(lexer, token) == ERROR)
		return (lexer_error(token), NULL);
	else if (!token->type && subshell_type(lexer, token) == ERROR)
		return (lexer_error(token), NULL);
	else if (!token->type && literal_type2(lexer, token) == ERROR)
		return (lexer_error(token), NULL);
	else if (!token->type)
		token->unknown = lexer->cur_char;
	//ft_fprintf(2, "token data: %s\n", token->str_data);
	return (token);
}

t_token	*next_new_token(t_lexer *lexer, bool recursive_call)
{
	t_token		*token;

	if (!lexer->str)
		return (NULL);
	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL);
	init_token(token, lexer);
	if (!classify_sub_str(token, lexer, recursive_call))
		return (NULL);
	if (token->type)
	{
		//if (!is_redir(token->type))
		return (read_char(lexer), token);
		//return (token);
	}
	printf("DEBUG: no function IDed the type\n");
	printf("%s\n", lexer->str + lexer->position);
	return (lexer_error(token), exit(1), NULL);
}

// util for new_lexer
void	skip_leading_void_whitespace(t_lexer *lexer)
{
	t_lexer	last;
	t_token	*token;

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
	*lexer = last;
}

// inits a lexer object, returns the object
// NOT a pointer to a dynamic memory location!!
t_lexer	new_lexer(char *str)
{
	t_lexer		lexer;

	lexer.last_char = 0;
	lexer.position = 0;
	lexer.read_position = 0;
	lexer.str = str;
	skip_leading_void_whitespace(&lexer);
	return (lexer);
}
