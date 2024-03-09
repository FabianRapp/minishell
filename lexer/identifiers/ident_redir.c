/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ident_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 00:06:31 by frapp             #+#    #+#             */
/*   Updated: 2024/03/09 04:00:01 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/lexer.h"
#include "../internals.h"

// util for redir_type
char	*get_potential_fd(t_lexer *lexer)
{
	char	*left_redir_arg;
	t_lexer	lexer_backup;

	lexer_backup = *lexer;
	left_redir_arg = NULL;
	while (ft_isdigit(lexer->cur_char))
	{
		if (!ft_strjoin_inplace_char(&left_redir_arg, lexer->cur_char))
			return (NULL);
		read_char(lexer);
	}
	return (left_redir_arg);
}

// TODO idk if here to check for larger fd than MAX_FD or let open handle that
char	*check_limis_potential_fd(char *left_redir_arg,
	t_lexer *lexer, t_lexer lexer_backup)
{
	if (lexer->cur_char != '<' && lexer->cur_char != '>')
		ft_free((void **)&left_redir_arg);
	else if (ft_strlen(left_redir_arg) > ft_strlen("2147483647"))
		ft_free((void **)&left_redir_arg);
	else if (ft_strlen(left_redir_arg) == ft_strlen("2147483647"))
	{
		if (ft_strcmp(left_redir_arg, "2147483647") > 0)
			ft_free((void **)&left_redir_arg);
	}
	if (!left_redir_arg)
		*lexer = lexer_backup;
	return (left_redir_arg);
}

t_result	handle_redir_fd(t_lexer *lexer, t_token *token)
{
	t_lexer	lexer_backup;

	if (ft_isdigit(lexer->cur_char))
	{
		lexer_backup = *lexer;
		token->left_redir_arg = get_potential_fd(lexer);
		if (!token->left_redir_arg)
			return (ERROR);
		token->left_redir_arg = check_limis_potential_fd
			(token->left_redir_arg, lexer, lexer_backup);
	}
	return (SUCCESS);
}

bool	is_redir_terminator_char(char c)
{
	if (ft_iswhitespace(c) || c == '|' || c == '&' || c == '('
		|| c == ')' || c == '\0' || c == '<' || c == '>')
	{
		return (true);
	}
	return (false);
}

bool	valid_redir_arg(t_lexer *redir_arg_start_lex)
{
	t_token	*error_token;

	while (ft_iswhitespace(redir_arg_start_lex->cur_char))
	{
		read_char(redir_arg_start_lex);
	}
	if (!is_redir_terminator_char(redir_arg_start_lex->cur_char))
		return (true);
	set_last_exit(2);
	error_token = next_new_token(redir_arg_start_lex, true);
	if (error_token)
	{
		print_error(true, NULL, NULL, type_to_str(error_token->type));
	}
	free_token(error_token);
	return (false);
}

t_result	lexer_here_doc(t_lexer *lexer, t_token *token)
{
	if (token->type != HERE_DOC)
		return (SUCCESS);
	while (!is_redir_terminator_char(lexer->cur_char))
	{
		if (!ft_strjoin_inplace_char(&(token->str_data), lexer->cur_char))
		{//todo error
		}
		read_char(lexer);
	}
	return (SUCCESS);
}

t_result	redir_type(t_lexer *lexer, t_token *token, bool recursive_call)
{
	if (handle_redir_fd(lexer, token) == ERROR)
		return (ERROR);
	if (lexer->cur_char == '<')
	{
		token->type = REDIR_IN;
		read_char(lexer);
		if (lexer->cur_char == '<')
		{
			read_char(lexer);
			token->type = HERE_DOC;
		}
	}
	else if (lexer->cur_char == '>')
	{
		token->type = REDIR_OUT;
		read_char(lexer);
		if (lexer->cur_char == '>')
		{
			read_char(lexer);
			token->type = REDIR_APPEND;
		}
	}
	if (!recursive_call && is_redir(token->type) && !valid_redir_arg(lexer))
		return (ERROR);
	return (lexer_here_doc(lexer, token));
}

// has to run after all other typechecks
t_result	literal_type2(t_lexer *lexer, t_token *token)
{
	if (is_termination_char(lexer->cur_char))
		return (SUCCESS);
	while (!is_termination_char((lexer->str)[lexer->read_position]))
	{
		(lexer->read_position)++;
	}
	token->type = LITERAL;
	token->str_data = ft_strndup(lexer->str
			+ lexer->position, lexer->read_position - lexer->position);
	if (!token->str_data)
		return (ERROR);
	return (SUCCESS);
}
