/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 08:54:59 by frapp             #+#    #+#             */
/*   Updated: 2024/03/23 16:34:26 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"
#include "internals_parser.h"
#include "../headers/lexer.h"

// syntax check for parse_redir_paths()
// if returns false exection of the current input ends
t_result	redirs_have_arg(t_parser *parser)
{
	while (parser->p_type != T_EOF)
	{
		if (is_redir(parser->p_type) && parser->arg == NULL && parser->p_type != HERE_DOC)
		{
			while (parser->next->p_type == WHITE_SPACE)
				parser = parser->next;
			print_error(true, NULL, NULL, type_to_str(parser->next->token->type));
			return (ERROR);
		}
		parser = parser->next;
	}
	return (SUCCESS);
}

t_result	parse_redir_paths(t_parser *parser)
{
	while (parser->p_type != T_EOF)
	{
		if (is_redir(parser->p_type) && parser->p_type != HERE_DOC)
		{
			move_to_arg(parser, is_redir_arg_terminator, REDIR_ARG, false);
		}
		parser = parser->next;
	}
	parser = parser->next;
	return (redirs_have_arg(parser));
}

void	type_args(t_parser *parser)
{
	t_parser	*last_command_start;

	last_command_start = parser;
	while (parser->p_type != T_EOF)
	{
		if (parser->p_type == COMMAND)
		{
			move_to_arg(parser, command_terminator, ARGUMENT, true);
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

	if (parser->token && parser->token->type == SUBSHELL && has_none_redir_arg(parser))
	{
		temp = ft_strjoin("syntax error near unexpected token ",
			has_none_redir_arg(parser)->token->str_data);
		print_error(true, NULL, NULL, temp);
		return (free(temp), set_last_exit(2), ERROR);
	}
	if ((parser->p_type == COMMAND || parser->p_type == SUBSHELL || parser->p_type == DUMMY_COMMAND) && in_command_block)
	{
		if (parser->p_type == COMMAND)
		{
			temp = ft_strjoin("syntax error near unexpected token ", parser->token->str_data);
			print_error(true, NULL, NULL, temp);
			free(temp);
		}
		else if (parser->p_type == SUBSHELL)
			print_error(true, NULL, NULL, "syntax error near unexpected token `('");
		else
			print_error(true, NULL, NULL, "Error");
		return (set_last_exit(2), ERROR);
	}
	return (SUCCESS);
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
				false, type_to_str(parser->p_type)), ERROR);
		if (is_operator(parser->p_type))
			in_command_block = false;
		parser = parser->next;
	}
	if (!in_command_block)
		return (set_last_exit(2), print_error(true, false,
			false, type_to_str(T_EOF)), ERROR);
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
