/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 08:54:59 by frapp             #+#    #+#             */
/*   Updated: 2024/02/10 23:07:57 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"
#include "internals_parser.h"
#include "../headers/lexer.h"

bool	parse_redir_paths(t_parser *parser)
{
	while (parser->p_type != T_EOF)
	{
		if (is_redir(parser->p_type))
		{
			if (parser->next->p_type == T_EOF)
				return (print_error(true, NULL, NULL, type_to_str(parser->next->token->type)), false);
			move_to_arg(parser, true, is_redir_arg_terminator, REDIR_ARG);
			 // TODO: parse possible words first so this does not have to be done, also in wrong var atm
		}
		parser = parser->next;
	}
	parser = parser->next; // reset to head and find syntax errors
	while (parser->p_type != T_EOF)
	{
		if (is_redir(parser->p_type) && parser->arg == NULL)
		{
			while (parser->next->p_type == WHITE_SPACE)
				parser = parser->next;
			return (print_error(true, NULL, NULL, type_to_str(parser->next->token->type)), false);
		}
		parser = parser->next;
	}
	return (true);
}

// retuns false on syntax error
bool	type_commands(t_parser *parser)
{
	bool		found_command;
	bool		redir;
	
	found_command = false;
	redir = false;
	while (parser->p_type != T_EOF)
	{
		if (is_redir(parser->p_type))
			redir = true;
		if (!found_command && !redir && is_operator(parser->p_type))
			return (print_error(true, NULL, NULL, type_to_str(parser->p_type)), false);
		else if (is_operator(parser->p_type))
		{
			if (!found_command && redir && !insert_dummy(parser))
				return (false);
			found_command = false;
			redir = false;
		}
		else if (!found_command && !is_operator(parser->p_type) && parser->p_type != WHITE_SPACE
				&& !is_redir(parser->p_type))
		{
			parser->p_type = COMMAND;
			found_command = true;
		}
		parser = parser->next;
	}
	if (!redir && !found_command)
	{
		print_error(true, NULL, NULL, type_to_str(T_EOF));
		return (false);
	}
	if (redir && !found_command && !insert_dummy(parser))
		return (false);
	return (true);
}

void	type_args(t_parser *parser)
{
	t_parser	*last_command_start;

	last_command_start = parser;
	while (parser->p_type != T_EOF)
	{
		if (parser->p_type == COMMAND)
		{
			move_to_arg(parser, true, command_terminator, ARGUMENT);
		}
		parser = parser->next;
	}
}

bool	merge_literals_parser(t_parser *parser)
{
	while(parser && parser->token->type != T_EOF)
	{
		while (parser->token->type == LITERAL && parser->next->token->type == LITERAL)
		{
			ft_strjoin_inplace(&(parser->token->str_data), parser->next->token->str_data);
			if (!parser->token->str_data)
				return (false);
			parser = parser->next;
			remove_parser_node(&parser, true);
		}
		if (parser && parser->token->type != T_EOF)
			parser = parser->next;
	}
	return (true);
}

void	merge_names(t_parser *parser)
{
	t_parser	*rest_name_end;

	while (parser->token->type != T_EOF)
	{
		if (!is_word_terminator(parser->token->type))
		{
			rest_name_end = parser->rest_name;
			while (rest_name_end && rest_name_end->next)
				rest_name_end = rest_name_end->next;
			while (!is_word_terminator(parser->next->token->type))
			{
				if (!rest_name_end)
				{
					parser->rest_name = parser->next;
					rest_name_end = parser->rest_name;
				}
				else
				{
					rest_name_end->next = parser->next;
					rest_name_end = rest_name_end->next;
				}
				parser->next = parser->next->next;
				rest_name_end->next = NULL;
			}
		}
		parser = parser->next;
	}
}

bool	empty_parser(t_parser *parser)
{
	t_parser	*head;

	if (!parser)
		return (true);
	head = parser;
	while (parser->p_type == WHITE_SPACE || parser->p_type == VOID
		|| parser->p_type == ERROR || parser->p_type == T_EOF)
	{
		parser = parser->next;
		if (parser == head)
		{
			free_parser_main(parser, true);
			return (true);
		}
	}
	return (false);
}

t_ast	*parser(char *str)
{
	t_parser	*parser;
	t_ast		*ast;

	if (!str)
		return (NULL);
	parser = init_parser(str);
	if (empty_parser(parser))
		return (NULL);
	trim_whitespace(parser);
	if (!merge_literals_parser(parser))
		return (free_parser_main(parser, true), NULL);
	merge_names(parser);
	if (!parse_redir_paths(parser))
		return (free_parser_main(parser, true), NULL);
	if (!type_commands(parser))
		return (free_parser_main(parser, true), NULL);
	remove_whitespace(parser);
	if (!move_commands_infront(parser))
	{
		free_parser_main(parser, true);
		return (NULL);
		// debug condtion in function will exit atm
	}
	//system("leaks minishell");
	jump_to_start(&parser);
	type_args(parser);
	//system("leaks minishell");
	ast = build_ast(parser);
	if (!ast)
	{
		// handle cleanup
	}
	
	return (ast);
}

// echo 'hello_word' | cat && (ls >> file > fil2 || echo 'ls failed') && cat < file && cat < file2
