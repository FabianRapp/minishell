/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:46:56 by frapp             #+#    #+#             */
/*   Updated: 2024/03/19 02:24:38 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/lexer.h"

// reads the next char into the lexer and updates indexes
void	read_char(t_lexer *lexer)
{
	lexer->last_char = lexer->cur_char;
	lexer->cur_char = (lexer->str)[lexer->read_position];
	lexer->position = lexer->read_position;
	if (lexer->read_position < ((int)ft_strlen(lexer->str)))
		(lexer->read_position)++;
}

void	init_token(t_token *token, t_lexer *lexer)
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

bool	is_redir_terminator_char(char c)
{
	if (ft_iswhitespace(c) || c == '|' || c == '&' || c == '('
		|| c == ')' || c == '\0' || c == '<' || c == '>')
	{
		return (true);
	}
	return (false);
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
