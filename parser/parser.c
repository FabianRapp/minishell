/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 08:54:59 by frapp             #+#    #+#             */
/*   Updated: 2024/01/22 20:57:35 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"
#include "internals_parser.h"
#include "../headers/lexer.h"

bool	stop_lexing(t_type type)
{
	if (type == HERE_DOC)
	{
		return (true);
	}
	else if (type == WILDCARD)
	{
		return (true);
	}
	return (false);
}

// returns the next token without moving the lexer
t_token	*peek_next_lexer_token(t_lexer *lexer)
{
	t_lexer	peek;
	t_token	*next_token;

	peek = *lexer;
	peek.read_position += 1;
	next_token = next_new_token(&peek);
	return (next_token);
}

// returns the type of the next token
t_type	peek_next_lexer_type(t_lexer *lexer)
{
	t_type	next_type;
	t_token	*next_token;

	next_token = peek_next_lexer_token(lexer);
	next_type = next_token->type;
	free_token(next_token);
	return (next_type);
}


void	skip_whitespace_lexer(t_lexer *lexer)
{
	while (peek_next_lexer_type(lexer) == WHITE_SPACE)
	{
		lexer->read_position += 1;
	}
}





bool	is_redir(t_type type)
{
	if (type == REDIR_IN
		|| type == REDIR_OUT
		|| type == REDIR_APPEND
		|| type == HERE_DOC)
	{
		return (true);
	}
	return (false);
}

bool	valid_path_syntax(t_token *token)
{
	int	len;

	if (!token->str_data)
		return (0);
	len = ft_strlen(token->str_data);
	if (!token->str_data || *(token->str_data) == 0 || len > PATH_MAX)
		return (false);
	// check max individual file name length:
	while (len && (token->str_data)[len - 1] != '/')
		len--;
	if (ft_strlen(token->str_data) - len > NAME_MAX)
		return (false);
	return (true);
}

// // currently does almost nothing
// bool	syntax_validity_command(t_token *command)
// {
// 	t_type	last_type;
// 	t_token	*cur;

// 	if (!command || command->type != COMMAND)
// 		return (false);
// 	// chech last token for invalid type of the last token of the command
// 	if (command->last->type ==)
// 		return (false);
// 	cur = command->next;
// 	// check for invalid order of types
// 	while(cur && cur->type != COMMAND)
// 	{
// 		last_type = cur->last->type;

// 		cur = cur->next;
// 	}
// 	// invalid cuz multiple commands or NULL ptr in circular list
// 	if (!cur || cur != command)
// 		return (false);
// }

// t_token	*parse_command(t_token *list)
// {
	
// 	if (list->type != WORD && cur_token->type != COMMAND)
// 	{
// 		if (DEBUG)
// 		{
// 			printf("INVALID OR ERROR: should be COMMAND or WORD:\n");
// 			print_token(*cur_token);
// 			printf("________________________________\n");
// 		}
// 		// OUTPUT INVALID COMMAND
// 		return (free_token(cur_token), NULL);
// 	}
// 	cur_token->type = COMMAND;
// 	add_token(&command, &command, cur_token); //t_token *cure_node add_token(**last_node, **head_node, *cur_node);
// 	next_type = peek_next_lexer_type(lexer);
// 	if (next_type != WHITE_SPACE)
// 		return (free_token(cur_token), NULL);
// 	skip_whitespace_lexer(lexer);
// 	next_type = peek_next_lexer_type(lexer);
// 	last_type = cur_token->type;
// 	while (next_type == WORD || next_type == FLAG || next_type == LITERAL
// 		|| next_type == WILDCARD ||  next_type == ENV_VAR || next_type == INTERPRETED
// 		|| is_redir(next_type))
// 	{
// 		last_type = cur_token->type;
// 		cur_token = add_token(&cur_token, &command, next_new_token(lexer));
// 		if (is_redir(last_type) && (cur_token->type == WORD || cur_token->type == FLAG))
// 		{
// 			cur_token->type == REDIR_ARG;
// 			while (peek_next_lexer_type(lexer) == FLAG)
// 			{
// 				temp = next_new_token(lexer);
// 				ft_strjoin_inplace(&(cur_token->str), temp->str);
// 				free_token(temp);
// 			}
// 		}
// 		else if (cur_token->type == WORD && last_type != WILDCARD && !is_redir(last_type))
// 			cur_token->type = ARGUMENT;
		
// 		skip_whitespace_lexer(lexer);
// 		next_type = peek_next_lexer_type(lexer);
// 	}
// 	if (!syntax_validity_command(command))
// 		return (free_token_list(command), NULL);
// 	return (command);
// }

void	trim_whitespace(t_parser *parser)
{
	bool	was_whitespace;

	was_whitespace = true;
	while(parser->type != T_EOF)
	{
		if (parser->type == WHITE_SPACE)
		{
			if (was_whitespace)
				remove_parser_node(&parser);
			was_whitespace = true;
		}
		else
			was_whitespace = false;
		parser = parser->next;
	}
}

void	validate_flags(t_parser *parser)
{
	bool	was_whitespace;

	was_whitespace = true;
	while(parser->type != T_EOF)
	{
		if (parser->type == FLAG)
		{
			if (!was_whitespace && parser->last->token->str_data)
			{
				ft_strjoin_inplace(&(parser->last->token->str_data), parser->token->str_data);
				remove_parser_node(&parser);
			}
		}
		if (parser->type == WHITE_SPACE)
			was_whitespace = true;
		else
			was_whitespace = false;
		parser = parser->next;
	}
}

// assumes parser is currently at a redir type node
// parses all supported redir arugments (for HERE_DOC parses the termination)
// the string will be stored in the str var of the token
// returns false on invalid input
bool	parse_redir_arg(t_parser *parser)
{
	int			len;
	t_parser	*arg_node;
	bool		found;
	char		*new_str_data;

	arg_node = parser;
	len = 0;
	if (parser->type == WHITE_SPACE)
	{
		parser = parser->next;
		found = true;
		(arg_node->token->input_position)++;
	}
	if (parser->type == T_EOF || parser->type == OR || parser->type == AND
		|| parser->type == T_EOF || parser->type == REDIR_IN || parser->type == REDIR_OUT
		|| parser->type == REDIR_APPEND || parser->type == HERE_DOC)
	{
		printf("invalid path:\t");
		print_token(*(parser->token));
		printf("\n");
		return (false);
	}
	while (parser->type != T_EOF && parser->type != OR && parser->type != AND
		&& parser->type != REDIR_IN && parser->type != REDIR_OUT
		&& parser->type != REDIR_APPEND && parser->type != HERE_DOC && parser->type != WHITE_SPACE)
	{
		if (found)
		{
			remove_parser_node(&parser);
		}
		parser = parser->next;
		found = true;
	}
	len += parser->token->input_position - arg_node->token->input_position;
	new_str_data = ft_strndup(arg_node->token->input_str + arg_node->token->input_position, len);
	if (!new_str_data)
		return (cleanup(), false);
	update_parser_node(arg_node, REDIR_ARG, new_str_data);
	return (true);
}

bool	parse_redir_paths(t_parser *parser)
{
	bool	return_val;

	return_val = true;
	while (parser->type != T_EOF)
	{
		if (is_redir(parser->type))
		{
			if (!parse_redir_arg(parser->next))
				return_val = false;
		}
		parser = parser->next;
	}
	return (return_val);
}

t_parser	*parser(char *str)
{
	t_parser	*parser;

	parser = init_parser(str);
	//trim_whitespace(parser);
	if (!parse_redir_paths(parser))
		printf("invlid path found\n");

	//validate_flags(list);
	return (parser);
}
