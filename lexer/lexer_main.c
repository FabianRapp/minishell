/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:42:58 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 23:05:40 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static t_result	valid_first_char(t_lexer *lexer)
{
	if (lexer->cur_char == ')')
	{
		print_error(true, NULL,
			NULL, "syntax error near unexpected token `)\'");
		set_last_exit(2);
		return (ERROR);
	}
	if (lexer->cur_char == '$'
		&& (lexer->str[lexer->position + 1] == '\''
			|| lexer->str[lexer->position + 1] == '\"'))
	{
		read_char(lexer);
	}
	return (SUCCESS);
}

static t_result	handle_exception_char(t_lexer *lexer, t_token *token)
{
	if (lexer->cur_char != '\\')
		return (SUCCESS);
	read_char(lexer);
	return (ident_wildcard_literals(lexer, token, true));
}

static t_token	*classify_sub_str(t_token *token, t_lexer *lexer,
	bool recursive_call)
{
	if (handle_exception_char(lexer, token) == ERROR)
		return (lexer_error(token), NULL);
	if (!token->type)
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
	else if (!token->type && ident_wildcard_literals(lexer, token,
			false) == ERROR)
		return (lexer_error(token), NULL);
	else if (!token->type)
		token->unknown = lexer->cur_char;
	return (token);
}

static void	init_token(t_token *token, t_lexer *lexer)
{
	token->type = 0;
	token->str_data = NULL;
	token->unknown = 0;
	token->input_str = lexer->str;
	token->input_position = lexer->position;
	token->old_data = NULL;
	token->left_redir_arg = NULL;
	token->here_doc_arg_literal = false;
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
		return (token);
	ft_fprintf(2, "%s: unexpected token: %s\n", SHELL_NAME, lexer->str + lexer->position);
	return (lexer_error(token), set_last_exit(2), NULL);
}
