/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 08:54:59 by frapp             #+#    #+#             */
/*   Updated: 2024/01/23 21:18:25 by frapp            ###   ########.fr       */
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

// // returns the next token without moving the lexer
// t_token	*peek_next_lexer_token(t_lexer *lexer)
// {
// 	t_lexer	peek;
// 	t_token	*next_token;

// 	peek = *lexer;
// 	peek.read_position += 1;
// 	next_token = next_new_token(&peek);
// 	return (next_token);
// }

// // returns the type of the next token
// t_type	peek_next_lexer_type(t_lexer *lexer)
// {
// 	t_type	next_type;
// 	t_token	*next_token;

// 	next_token = peek_next_lexer_token(lexer);
// 	next_type = next_token->type;
// 	free_token(next_token);
// 	return (next_type);
// }
// void	skip_whitespace_lexer(t_lexer *lexer)
// {
// 	while (peek_next_lexer_type(lexer) == WHITE_SPACE)
// 	{
// 		lexer->read_position += 1;
// 	}
// }

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
// 	add_token_back(&command, &command, cur_token); //t_token *cure_node add_token_back(**last_node, **head_node, *cur_node);
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
// 		cur_token = add_token_back(&cur_token, &command, next_new_token(lexer));
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

	was_whitespace = false;
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

// does not remove all wrong flags, potential wrong flags left:
// -flags before commands -> while indentifing words as commands flags have to be treated as words
void	remove_wrong_flags(t_parser *parser)
{
	t_parser	*last;

	last = NULL;
	while(parser->type != T_EOF)
	{
		if (parser->type == FLAG)
		{
			if (last && (last->type == WORD || last->type == ENV_VAR || last->type == FT_BUILDIN
				|| last->type == FLAG))
			{
				ft_strjoin_inplace(&(last->token->str_data), parser->token->str_data);
				if (!last->token->str_data)
				{
					cleanup();
					return ;
				}
				remove_parser_node(&parser);
			}
		}
		parser = parser->next;
	}
}

/*
	moves the next nodes of the parser list to the argument list of the given node
*/
void	move_to_arg(t_parser *parser, bool skip_first_whitespace, bool is_terminator(t_type))
{
	int			len;
	t_parser	*node;
	t_parser	*arg_data;

	node = parser;
	arg_data = parser->arg;
	len = 0;
	parser = parser->next;
	while (skip_first_whitespace && parser->type == WHITE_SPACE)
	{
		remove_parser_node(&parser);
		parser = parser->next;
	}
	while (arg_data && arg_data->next)
		arg_data = arg_data->next;
	while (!is_terminator(parser->type))
	{
		if (arg_data)
		{
			arg_data->next = parser;
			arg_data = arg_data->next;
		}
		else
		{
			arg_data = parser;
			node->arg = arg_data;
		}
		parser = parser->next;
	}
	node->next = arg_data->next;
	arg_data->next = NULL;
}

void	parse_redir_paths(t_parser *parser)
{
	while (parser->type != T_EOF)
	{
		if (is_redir(parser->type))
		{
			move_to_arg(parser, true, is_redir_arg_terminator);
			//optional TODO: remove single quotes of literals
		}
		parser = parser->next;
	}
}

bool	type_commands(t_parser *parser)
{
	bool	found_command;
	int		len;

	found_command = false;
	len = 0;
	while (parser->type != T_EOF)
	{
		len++;
		if (!found_command && is_operator(parser->type))
		{
			//bash: syntax error near unexpected parser->token
			return (false);
		}
		if (found_command && is_operator(parser->type))
			found_command = false;
	}
	if (len && !found_command)
	{
		//bash: syntax error: unexpected end of file
		return (false);
	}
	return (true);
}

// bool	syntax_error_brackets(t_parser *parser)
// {
// 	return (false);
// }

/*
TODO:
	lex interpreted strings and merge them
*/
t_parser	*parser(char *str)
{
	t_parser	*parser;

	parser = init_parser(str);
	//lex interpreted strings
	trim_whitespace(parser);
	//remove_wrong_flags(parser);
	parse_redir_paths(parser);
	//type_commands(parser);
	return (parser);
}
