/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 08:54:59 by frapp             #+#    #+#             */
/*   Updated: 2024/02/12 18:08:13 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"
#include "internals_parser.h"
#include "../headers/lexer.h"

// syntax check for parse_redir_paths()
// if returns false exection of the current input ends
t_redir	redirs_have_arg(t_parser *parser)
{
	while (parser->p_type != T_EOF)
	{
		if (is_redir(parser->p_type) && parser->arg == NULL)
		{
			while (parser->next->p_type == WHITE_SPACE)
				parser = parser->next;
			print_error(true, NULL, NULL, type_to_str(parser->next->token->type));
			return (FAIL_ERROR);
		}
		parser = parser->next;
	}
	return (SUCCESS);
}

t_result	parse_redir_paths(t_parser *parser)
{
	while (parser->p_type != T_EOF)
	{
		if (is_redir(parser->p_type))
			move_to_arg(parser, is_redir_arg_terminator, REDIR_ARG);
		parser = parser->next;
	}
	parser = parser->next; // reset to head of circular list
	return (redirs_have_arg(parser));
}

// abstraction for type_commands
static bool	handle_operator(t_parser *parser, bool *found_command, bool *found_redir)
{
	if (!is_operator(parser->p_type))
		return (SUCCESS);
	if (!*found_command && !*found_redir && is_operator(parser->p_type))
		return (print_error(true, NULL, NULL, type_to_str(parser->p_type)), FAIL_ERROR);
	if (!*found_command && *found_redir && !insert_dummy(parser))
		return (FAIL_ERROR);
	*found_command = false;
	*found_redir = false;
	return (SUCCESS);
}

// abstraction for type_commands()
// return value is just for readability and not used
static void	type_command(t_parser *parser, bool *found_command)
{
	if (is_operator(parser->p_type) || *found_command || parser->p_type == WHITE_SPACE
		|| is_redir(parser->p_type))
	{
		return ;
	}
	parser->p_type = COMMAND;
	*found_command = true;
}

t_result	type_commands(t_parser *parser)
{
	bool		found_command;
	bool		found_redir;

	found_command = false;
	found_redir = false;
	while (parser->p_type != T_EOF)
	{
		found_redir = is_redir(parser->p_type);
		if (!handle_operator(parser, &found_command, &found_redir))
			return (FAIL_ERROR);
		type_command(parser, &found_command);
		parser = parser->next;
	}
	if (!found_redir && !found_command)
		return (print_error(true, NULL, NULL, type_to_str(T_EOF)), FAIL_ERROR);
	if (found_redir && !found_command && !insert_dummy(parser))
		return (FAIL_ERROR);
	return (SUCCESS);
}

void	type_args(t_parser *parser)
{
	t_parser	*last_command_start;

	last_command_start = parser;
	while (parser->p_type != T_EOF)
	{
		if (parser->p_type == COMMAND)
		{
			move_to_arg(parser, command_terminator, ARGUMENT);
		}
		parser = parser->next;
	}
}

// merges concurrent literal nodes to one node
t_result	merge_literals_parser(t_parser *parser)
{
	while(parser && parser->token->type != T_EOF)
	{
		while (parser->token->type == LITERAL && parser->next->token->type == LITERAL)
		{
			ft_strjoin_inplace(&(parser->token->str_data), parser->next->token->str_data);
			if (!parser->token->str_data)
				return (FAIL_ERROR);
			parser = parser->next;
			remove_parser_node(&parser, true);
		}
		if (parser && parser->token->type != T_EOF)
			parser = parser->next;
	}
	return (SUCCESS);
}

// merges nodes that are next to each other without sepertator to one node
// these nodes will later be the strings that are the commands and arguments
void	merge_names(t_parser *parser)
{
	t_parser	*rest_name_end;

	while (parser->token->type != T_EOF)
	{
		if (!is_word_terminator(parser->token->type))
		{
			rest_name_end = parser->rest_name;
			while (rest_name_end && rest_name_end->next)
			{
				rest_name_end = rest_name_end->next;
			}
			while (!is_word_terminator(parser->next->token->type))
			{
				move_next_to_name(parser, &rest_name_end);
			}
		}
		parser = parser->next;
	}
}

t_result	has_content(t_parser *parser)
{
	t_parser	*head;

	if (!parser)
		return (FAIL_ERROR);
	head = parser;
	while (parser->p_type == WHITE_SPACE || parser->p_type == VOID
		|| parser->p_type == ERROR || parser->p_type == T_EOF)
	{
		parser = parser->next;
		if (parser == head)
		{
			free_parser_main(parser, true);
			return (FAIL_ERROR);
		}
	}
	return (SUCCESS);
}

t_ast	*parser(char *str)
{
	t_parser	*parser;
	t_ast		*ast;

	if (!str)
		return (NULL);
	parser = init_parser(str);
	if (has_content(parser) == FAIL_ERROR)
		return (NULL);
	trim_whitespace(parser);
	if (merge_literals_parser(parser) == FAIL_ERROR)
		return (free_parser_main(parser, true), NULL);
	merge_names(parser);
	if (parse_redir_paths(parser) == FAIL_ERROR)
		return (free_parser_main(parser, true), NULL);
	if (type_commands(parser) == FAIL_ERROR)
		return (free_parser_main(parser, true), NULL);
	remove_whitespace(parser);
	if (move_commands_infront(parser) == FAIL_ERROR)
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
