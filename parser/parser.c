/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 08:54:59 by frapp             #+#    #+#             */
/*   Updated: 2024/01/24 22:29:47 by frapp            ###   ########.fr       */
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
	while(parser->p_type != T_EOF)
	{
		if (parser->p_type == WHITE_SPACE)
		{
			if (was_whitespace)
				remove_parser_node(&parser, true);
			was_whitespace = true;
		}
		else
			was_whitespace = false;
		parser = parser->next;
	}
}

// // does not remove all wrong flags, potential wrong flags left:
// // -flags before commands -> while indentifing words as commands flags have to be treated as words
// void	remove_wrong_flags(t_parser *parser)
// {
// 	t_parser	*last;

// 	last = NULL;
// 	while(parser->p_type != T_EOF)
// 	{
// 		if (parser->p_type == FLAG)
// 		{
// 			if (last && (last->type == WORD || last->type == ENV_VAR || last->type == FT_BUILDIN
// 				|| last->type == FLAG))
// 			{
// 				ft_strjoin_inplace(&(last->token->str_data), parser->token->str_data);
// 				if (!last->token->str_data)
// 				{
// 					cleanup();
// 					return ;
// 				}
// 				remove_parser_node(&parser);
// 			}
// 		}
// 		parser = parser->next;
// 	}
// }

/*
	moves the next nodes of the parser list to the argument list of the given node
*/
void	move_to_arg(t_parser *parser, bool skip_first_whitespace,
			bool is_terminator(t_type), t_type new_type, bool skip_redir)
{
	int			len;
	t_parser	*node;
	t_parser	*arg_data;
	t_type		type;

	node = parser;
	arg_data = parser->arg;
	len = 0;
	parser = parser->next;
	type = parser->p_type;

	while (skip_first_whitespace && parser->p_type == WHITE_SPACE)
	{
		remove_parser_node(&parser, true);
		parser = parser->next;
	}
	while (arg_data && arg_data->next && arg_data->p_type != T_EOF)
		arg_data = arg_data->next;
	while (skip_redir && is_redir(parser->p_type))
		parser = parser->next;
	while (!is_terminator(parser->p_type))
	{
		parser->p_type = new_type;
		//parser->p_type = type;
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
		while (skip_redir && is_redir(parser->p_type))
			parser = parser->next;
	}
	if (arg_data)
	{
		node->next = arg_data->next;
		arg_data->next = NULL;
	}
}

bool	parse_redir_paths(t_parser *parser)
{
	while (parser->p_type != T_EOF)
	{
		if (is_redir(parser->p_type))
		{
			if (parser->next->p_type == T_EOF)
				return (false); //sytanx error unecpected EOF
			move_to_arg(parser, true, is_redir_arg_terminator, REDIR_ARG, false); // TODO: parse possible words first so this does not have to be done, also in wrong var atm
			//optional TODO: remove single quotes of literals
		}
		parser = parser->next;
	}
	parser = parser->next; // reset to head
	while (parser->p_type != T_EOF)
	{
		if (is_redir(parser->p_type) && parser->arg == NULL)
		{
			// syntax error unexpected sign
			return (false);
		}
		parser = parser->next;
	}
	return (true);
}

// retuns false on syntax error
bool	type_commands(t_parser *parser)
{
	bool		found_command;
	int			len;
	//t_parser	*last;

	found_command = false;
	len = 0;
	while (parser->p_type != T_EOF)
	{
		if (is_redir(parser->p_type))
			len++;
		if (!found_command && is_operator(parser->p_type))
			//bash: syntax error near unexpected parser->token
			return (false);
		else if (found_command && is_operator(parser->p_type))
			found_command = false;
		else if (!found_command && !is_operator(parser->p_type) && parser->p_type != WHITE_SPACE
				&& !is_redir(parser->p_type))
		{
			len++;
			parser->p_type = COMMAND;
			found_command = true;
		}
		parser = parser->next;
	}
	if (len && !found_command)
	{
		//bash: syntax error: unexpected end of file
		return (false);
	}
	return (true);
}

bool	type_args(t_parser *parser)
{
	while (parser->p_type != T_EOF)
	{
		if (parser->p_type == COMMAND)
		{
			move_to_arg(parser, true, command_terminator, ARGUMENT, true);
		}
		parser = parser->next;
	}
	return (true);
}

void	merge_words(t_parser *parser)
{
	while (parser->p_type != T_EOF)
	{
		if (!is_word_terminator(parser->p_type))
		{
			move_to_arg(parser, false, is_word_terminator, WORD, false);
			parser->rest_name = parser->arg;
			parser->arg = NULL;
		}
		parser=parser->next;
	}
}

void	remove_whitespace(t_parser *parser)
{
	while (parser->p_type != T_EOF)
	{
		if (parser->p_type == WHITE_SPACE)
		{
			remove_parser_node(&parser, true);
		}
		parser = parser->next;
	}
}

// does not clean up the parser
// uses tokens of parser -->> dont free tokens when freeing parser
t_token_list	*extract_token_list(t_parser *parser, char name_or_arg)
{
	t_token_list	*last;
	t_token_list	*new_list;

	if (!parser || parser->p_type == T_EOF || !parser->token || parser->token->type == T_EOF)
		return (NULL);
	new_list = ft_calloc(1, sizeof(t_token_list));
	if (!new_list)
		return (cleanup(), NULL);
	new_list->token = parser->token;
	if (name_or_arg == ARG)
		new_list->next = extract_token_list(parser->arg, RECURSIVE_CALL);
	else if (name_or_arg == NAME)
		new_list->next = extract_token_list(parser->rest_name, RECURSIVE_CALL);
	else // if (name_or_arg == RECURSIVE_CALL)
		new_list->next = extract_token_list(parser->next, RECURSIVE_CALL);
	if (DEBUG_EXTRACT_TOKENS && (name_or_arg == ARG || name_or_arg == NAME))
	{
		last = new_list;
		int i = 0;
		while (last)
		{
			printf("extracted token %d: \n", i++);
			print_token(last->token, NULL, 0);
			printf("\n");
			last = last->next;
		}
	}
	return (new_list);
}

t_token_list	*extract_command_name(t_parser *parser, t_parser **command_start)
{
	t_token_list	*list;

	if (!parser || !command_start)
		return (NULL);
	*command_start = parser;
	if ((*command_start)->p_type == COMMAND)
		(*command_start) = (*command_start)->next;
	while(parser->p_type != COMMAND && !command_terminator(parser->p_type))
		parser = parser->next;
	if (command_terminator(parser->p_type))
		return (NULL);
	list = extract_token_list(parser, NAME);
	remove_parser_node(&parser, false);
	return (list);
}

t_arg	*append_tokens(t_arg *arg, t_parser *parser)
{
	t_arg	*new_node;

	new_node = ft_calloc(1, sizeof(t_arg));
	if (!new_node)
		return (cleanup(), NULL);
	
	return (new_node);
}
t_ast_command	*build_ast_command(t_parser *parser)
{
	t_ast_command	*ast_command;
	//t_parser		*command_start;
	//t_parser		*last;

	while (command_terminator(parser->p_type) && parser->p_type != T_EOF)
		parser = parser->next;
	if (parser->p_type == T_EOF)
		return (NULL);
	ast_command = malloc(sizeof(t_ast_command));
	if (!ast_command)
		return (cleanup(), NULL);
	while (!command_terminator(parser->p_type))
	{
		if (parser->p_type == COMMAND)
		{
			if (ast_command->name)
			{
				// syntax error
			}
			ast_command->name = extract_token_list(parser, NAME);
			remove_parser_node(&parser, false);
		}
		else if (parser->p_type == REDIR_IN || parser->p_type == HERE_DOC)
		{

		}
		else if (parser->p_type == REDIR_OUT || parser->p_type == REDIR_APPEND)
		{
		}
		else if (parser->p_type == ARGUMENT)
		{
		}
		parser = parser->next;
	}
	/*
		TODO here:
		malloc fail handle
		syntax erro echecks
	*/

		
	// // extract name
	// ast_command->name = extract_command_name(parser, &command_start);
	// if (!ast_command->name)
	// 	return (free(ast_command), cleanup(), NULL);
	// //extract redir in
	// parser = command_start;
	// ast_command->redir_in = extract_command_name(parser, &command_start);
	// if (!ast_command->redir_in)
	// 	return (free(ast_command), cleanup(), NULL);
	// // extract redir out
	// parser = command_start;
	// ast_command->redir_out = extract_command_name(parser, &command_start);
	// if (!ast_command->name)
	// 	return (free(ast_command), cleanup(), NULL);
	// // extract args
	// parser = command_start;
	// ast_command->redir_out = extract_command_name(parser, &command_start);
	// if (!ast_command->name)
	// 	return (free(ast_command), cleanup(), NULL);
	return (ast_command);
}

/*
TODO:
	lex interpreted strings and merge them
*/
t_parser	*parser(char *str)
{
	t_parser	*parser;

	parser = init_parser(str);
	//TODO: parse words: literals and interpreted strings and what ever can be combined to 1 token: word
	merge_words(parser);
	trim_whitespace(parser);
	parse_redir_paths(parser);
	if (!type_commands(parser))
	{
		// handle syntax error
	}
	remove_whitespace(parser);
	if (!type_args(parser))
	{
		// handle syntax error
	}
	build_ast_command(parser);
	//if (!init_ast(&parser))
	{
		// handle cleanup
	}
	return (parser);
}

// echo 'hello_word' | cat && (ls >> file > fil2 || echo 'ls failed') && cat < file && cat < file2

// LogicalAND
// ├── Pipeline
// │   ├── Command
// │   │   └── SimpleCommand
// │   │       ├── CommandName (echo)
// │   │       └── CommandArguments ('hello_word')
// │   └── Command
// │       └── SimpleCommand
// │           └── CommandName (cat)
// ├── LogicalAND
// │   ├── Subshell
// │   │   └── LogicalOR
// │   │       ├── Pipeline
// │   │       │   └── Command
// │   │       │       └── SimpleCommand
// │   │       │           ├── CommandName (ls)
// │   │       │           ├── Redirection
// │   │       │           │   ├── Operator (>>)
// │   │       │           │   └── Word (file)
// │   │       │           └── Redirection
// │   │       │               ├── Operator (>)
// │   │       │               └── Word (fil2)
// │   │       └── Command
// │   │           └── SimpleCommand
// │   │               ├── CommandName (echo)
// │   │               └── CommandArguments ('ls failed')
// │   └── Pipeline
// │       └── Command
// │           └── SimpleCommand
// │               ├── CommandName (cat)
// │               └── Redirection
// │                   ├── Operator (<)
// │                   └── Word (file)
// └── Pipeline
//     └── Command
//         └── SimpleCommand
//             ├── CommandName (cat)
//             └── Redirection
//                 ├── Operator (<)
//                 └── Word (file2)
