/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ident_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:29:01 by frapp             #+#    #+#             */
/*   Updated: 2024/02/14 13:15:34 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/lexer.h"
#include "../internals.h"

char	*extract_str_data(t_lexer *lexer)
{
	char	*str_data;
	int		len;

	len = lexer->read_position - lexer->position;
	str_data = ft_strndup(lexer->str + lexer->position, len);
	return (str_data);
}
/*
	utils to fill the type if the token should be ignored in case
	of no str_data (in that case the void type is entered so the
	parser ignores this token)
*/
void	type_token_with_void_check(t_token *token, t_type type)
{
	if (!token)
		return ;
	if (token->str_data == NULL || ft_strlen(token->str_data) == 0)
	{
		my_free((void **)&(token->str_data));
		token->type = VOID;
	}
	else
		token->type = type;
}

void	basic_sign_type(t_lexer *lexer, t_token *token)
{
	if (lexer->cur_char == 0)
		token->type = T_EOF;
	else if (ft_iswhitespace(lexer->cur_char))
		token->type = WHITE_SPACE;
	else if (lexer->cur_char == '&' && (lexer->str)[lexer->position + 1] == '&')
	{
		token->type = AND;
		lexer->read_position = lexer->position + 2;
	}
	else if (lexer->cur_char == '|' && (lexer->str)[lexer->position + 1] == '|')
	{
		token->type = OR;
		lexer->read_position = lexer->position + 2;
	}
	else if (lexer->cur_char == '|')
		token->type = PIPE;
	else if (lexer->cur_char == '$' && (lexer->str)[lexer->position + 1] == '?')
	{
		token->type = EXIT_STATUS_REQUEST;
		lexer->read_position = lexer->position + 2;
	}
	else if (lexer->cur_char == '*')
		token->type = WILDCARD;
}

t_result	literal_type(t_lexer *lexer, t_token *token)
{
	if (lexer->cur_char != '\'')
		return (SUCCESS);
	while ((lexer->str)[lexer->read_position] && (lexer->str)[lexer->read_position] != '\'')
	{
		(lexer->read_position)++;
	}
	if (!(lexer->str)[lexer->read_position])
	{
		ft_fprintf(2, "exit\n");
		print_error(true, "debug literal_type", NULL, "unexpected EOF while looking for matching `\'\'");
		exit(2);
	}
	lexer->position++;
	token->str_data = extract_str_data(lexer);
	if (!token->str_data)
		return (ERROR);
	lexer->read_position++;
	type_token_with_void_check(token, LITERAL);
	return (SUCCESS);
}

t_result	interpreted_type(t_lexer *lexer, t_token *token)
{
	if (lexer->cur_char != '\"')
		return (SUCCESS);
	while ((lexer->str)[lexer->read_position] && (lexer->str)[lexer->read_position] != '\"')
	{
		(lexer->read_position)++;
	}
	if (!(lexer->str)[lexer->read_position])
	{
		ft_fprintf(2, "exit\n");
		print_error(true, "debug interpreted_type", NULL, "unexpected EOF while looking for matching `\"\'");
		exit(2);
	}
	lexer->position++;
	token->str_data = extract_str_data(lexer);
	if (!token->str_data)
		return (ERROR);
	lexer->read_position++;
	type_token_with_void_check(token, INTERPRETED);
	return (SUCCESS);
}

// util for redir_type
char	*get_potential_fd(t_lexer *lexer)
{
	char	*potential_fd;
	t_lexer	lexer_backup;

	lexer_backup = *lexer;
	potential_fd = NULL;
	while (ft_isdigit(lexer->cur_char))
	{
		if (!ft_strjoin_inplace_char(&potential_fd, lexer->cur_char))
			return (NULL);
		read_char(lexer);
	}
	return (potential_fd);
}

// TODO idk if here to check for larger fd than MAX_FD or let open handle that
char	*check_limis_potential_fd(char *potential_fd, t_lexer *lexer, t_lexer lexer_backup)
{
	if (lexer->cur_char != '<' && lexer->cur_char != '>')
		my_free((void **)&potential_fd);
	else if (ft_strlen(potential_fd) > ft_strlen("2147483647"))
		my_free((void **)&potential_fd);
	else if (ft_strlen(potential_fd) == ft_strlen("2147483647"))
	{
		if (ft_strcmp(potential_fd, "2147483647") > 0)
			my_free((void **)&potential_fd);
	}
	if (!potential_fd)
		*lexer = lexer_backup;
	return (potential_fd);
}

t_result	redir_type(t_lexer *lexer, t_token *token)
{
	t_lexer	lexer_backup;

	if (ft_isdigit(lexer->cur_char))
	{
		lexer_backup = *lexer;
		token->potential_fd = get_potential_fd(lexer);
		if (!token->potential_fd)
			return (ERROR);
		token->potential_fd = check_limis_potential_fd(token->potential_fd, lexer, lexer_backup);
	}
	
	if (lexer->cur_char == '<')
	{
		if ((lexer->str)[lexer->position + 1] == '<')
		{
			token->type = HERE_DOC;
			lexer->read_position += 1;
		}
		else
			token->type = REDIR_IN;
	}
	else if (lexer->cur_char == '>')
	{
		if ((lexer->str)[lexer->position + 1] == '>')
		{
			token->type = REDIR_APPEND;
			lexer->read_position += 1;
		}
		else
			token->type = REDIR_OUT;
	}
	return (SUCCESS);
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
	token->str_data = ft_strndup(lexer->str + lexer->position, lexer->read_position - lexer->position);
	if (!token->str_data)
		return (ERROR);
	return (SUCCESS);
}
