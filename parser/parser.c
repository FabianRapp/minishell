/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 08:54:59 by frapp             #+#    #+#             */
/*   Updated: 2024/03/09 02:29:20 by frapp            ###   ########.fr       */
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
		// if (is_redir(parser->p_type) && parser->arg == NULL)// && parser->p_type != HERE_DOC)
		if (is_redir(parser->p_type) && parser->arg == NULL && parser->p_type != HERE_DOC)
		{
			while (parser->next->p_type == WHITE_SPACE)
				parser = parser->next;
			print_error(true, "debug redirs_have_arg", NULL, type_to_str(parser->next->token->type));
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
		// if (is_redir(parser->p_type))// && parser->p_type != HERE_DOC)
		if (is_redir(parser->p_type) && parser->p_type != HERE_DOC)
		{
			move_to_arg(parser, is_redir_arg_terminator, REDIR_ARG, false);
		}
		parser = parser->next;
	}
	parser = parser->next; // reset to head of circular list
	return (redirs_have_arg(parser));
}

void	type_args(t_parser *parser)
{
	t_parser	*last_command_start;

	last_command_start = parser;
	while (parser->p_type != T_EOF)
	{
		if (parser->p_type == COMMAND)// && parser->token->type != SUBSHELL)
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

t_result	has_content(t_parser *parser)
{
	t_parser	*head;

	if (!parser)
		return (ERROR);
	head = parser;
	while (parser->p_type == WHITE_SPACE || parser->p_type == VOID
		|| parser->p_type == T_EOF)
	{
		parser = parser->next;
		if (parser == head)
		{
			free_parser_main(parser, true);
			return (ERROR);
		}
	}
	return (SUCCESS);
}


bool	has_redir_arg(t_parser *parser)
{
	t_parser	*cur_arg;

	cur_arg = parser->arg;
	while (cur_arg)
	{
		if ((is_redir(cur_arg->p_type) || is_redir(cur_arg->token->type)) && cur_arg->token->type != HERE_DOC)
			return (true);
		cur_arg = cur_arg->next;
	}
	return (false);
}

t_parser	*has_none_redir_arg(t_parser *parser)
{
	t_parser	*cur_arg;

	cur_arg = parser->arg;
	while (cur_arg)
	{
		if (!(is_redir(cur_arg->p_type) || is_redir(cur_arg->token->type)))
			return (cur_arg);
		cur_arg = cur_arg->next;
	}
	return (NULL);
}

// TODO: dosnt work: would print error for echo (echo)
t_result	validate_command_oder(t_parser *parser)
{
	bool	in_command_block;
	char	*temp;

	if (!parser)
		return (ERROR);
	in_command_block = false;
	//print_parser(parser, 0);
	//printf("validate_command_oder\n");
	while (parser && parser->p_type != T_EOF)
	{
		if (parser->token && parser->token->type == SUBSHELL && has_none_redir_arg(parser))
		{
			temp = ft_strjoin("syntax error near unexpected token ", has_none_redir_arg(parser)->token->str_data);
			print_error(true, NULL, NULL, temp);
			set_last_exit(2);
			return (ERROR);
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
			{
				print_error(true, "DEBUG 3", NULL, "syntax error near unexpected token `('");
			}
			else
				print_error(true, NULL, NULL, "Error");
			set_last_exit(2);
			return (ERROR);
		}
		if (parser->p_type == COMMAND || parser->p_type == SUBSHELL || parser->p_type == DUMMY_COMMAND)
			in_command_block = true;
		if (is_operator(parser->p_type) && !in_command_block)
			return (set_last_exit(2), print_error(true, "debug test123", false, type_to_str(parser->p_type)), ERROR);
		if (is_operator(parser->p_type))
			in_command_block = false;
		parser = parser->next;
	}
	if (!in_command_block)
		return (set_last_exit(2), print_error(true, false, false, type_to_str(T_EOF)), ERROR);
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
	if (type_commands(parser) == ERROR)//
		return (free_parser_main(parser, true), NULL);
	move_commands_infront(parser);
	type_args(parser);
	if (validate_command_oder(parser) == ERROR)
		return (free_parser_main(parser, true), NULL);
	//printf("full parser:\n");
	//print_parser(parser, 0);
	return (build_ast(parser));
}

t_parser	*parser_testing(char *str)
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
	//return (parser);
	move_commands_infront(parser);
	return (parser);
	type_args(parser);
	
	//system("leaks minishell");
}

// echo 'hello_word' | cat && (ls >> file > fil2 || echo 'ls failed') && cat < file && cat < file2
