/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 08:54:59 by frapp             #+#    #+#             */
/*   Updated: 2024/03/26 04:29:20 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"
#include "internals_parser.h"
#include "../headers/lexer.h"

// merges concurrent literal nodes to one node
t_result	merge_literals_parser(t_parser *parser)
{
	while (parser && parser->token->type != T_EOF)
	{
		while (parser->token->type == LITERAL
			&& parser->next->token->type == LITERAL)
		{
			ft_strjoin_inplace(&(parser->token->str_data),
				parser->next->token->str_data);
			if (!parser->token->str_data)
				return (ERROR);
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
		if (is_word_terminator(parser->token->type) == false)
		{
			rest_name_end = parser->rest_name;
			while (rest_name_end && rest_name_end->next)
			{
				rest_name_end = rest_name_end->next;
			}
			while (is_word_terminator(parser->next->token->type) == false)
			{
				move_next_to_restname(parser, &rest_name_end);
			}
		}
		parser = parser->next;
	}
}

t_result	check_error_valid_order(t_parser *parser, bool in_command_block)
{
	char	*temp;

	if (parser->token && parser->token->type == SUBSHELL
		&& has_none_redir_arg(parser))
	{
		temp = ft_strjoin("syntax error near unexpected token ",
				has_none_redir_arg(parser)->token->str_data);
		print_error(true, NULL, NULL, temp);
		return (free(temp), set_last_exit(2), ERROR);
	}
	if (!((parser->p_type == COMMAND || parser->p_type == SUBSHELL
				|| parser->p_type == DUMMY_COMMAND) && in_command_block))
		return (SUCCESS);
	set_last_exit(2);
	if (parser->p_type == COMMAND)
		return (print_error(true, NULL, NULL,
				type_to_error(parser->token->type)), ERROR);
	else if (parser->p_type == SUBSHELL)
		return (print_error(true, NULL, NULL,
				"syntax error near unexpected token `('"), ERROR);
	return (print_error(true, NULL, NULL, "Error"), ERROR);
}

static t_result	validate_command_order(t_parser *parser)
{
	bool	in_command_block;

	if (!parser)
		return (ERROR);
	in_command_block = false;
	while (parser && parser->p_type != T_EOF)
	{
		if (check_error_valid_order(parser, in_command_block) == ERROR)
			return (ERROR);
		if (parser->p_type == COMMAND || parser->p_type == SUBSHELL
			|| parser->p_type == DUMMY_COMMAND)
			in_command_block = true;
		if (is_operator(parser->p_type) && !in_command_block)
			return (set_last_exit(2), print_error(true, NULL,
					false, type_to_error(parser->p_type)), ERROR);
		if (is_operator(parser->p_type))
			in_command_block = false;
		parser = parser->next;
	}
	if (!in_command_block)
		return (set_last_exit(2), print_error(true, false,
				false, type_to_error(T_EOF)), ERROR);
	return (SUCCESS);
}

t_ast	*parser(char *str)
{
	t_parser	*parser;

	if (!str)
		return (NULL);
	parser = init_parser(str);
	if (has_content(parser) == ERROR)
		return (NULL);
	trim_whitespace(parser);
	if (merge_literals_parser(parser) == ERROR)
		return (free_parser_main(parser, true), NULL);
	merge_names(parser);
	remove_whitespace(parser);
	if (parse_redir_paths(parser) == ERROR)
		return (free_parser_main(parser, true), NULL);
	if (type_commands(parser) == ERROR)
		return (free_parser_main(parser, true), NULL);
	move_commands_infront(parser);
	type_args(parser);
	if (validate_command_order(parser) == ERROR)
		return (free_parser_main(parser, true), NULL);
	return (build_ast(parser));
}
