/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 08:54:59 by frapp             #+#    #+#             */
/*   Updated: 2024/02/10 21:59:47 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"
#include "internals_parser.h"
#include "../headers/lexer.h"

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

/*
	moves the next nodes of the parser list to the argument list of the given node
*/
void	move_to_arg(t_parser *parser, bool skip_first_whitespace,
			bool is_terminator(t_type), t_type new_type)
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

// util for type_commands()
bool	insert_dummy(t_parser *parser)
{
	t_parser	*dummy;

	dummy = ft_calloc(1, sizeof(t_parser));
	if (!dummy)
		return (false);
	dummy->next = parser->next;
	dummy->p_type = COMMAND;
	dummy->token = new_dummy_token();
	if (!dummy->token)
		return (free(dummy), false);
	dummy->token->type = DUMMY_COMMAND;
	parser->next = dummy;
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

void	swap_parsers(t_parser *node1, t_parser *node2)
{
	t_parser	*old1_next;
	t_parser	*old2_next;
	t_parser	*old1_last;
	t_parser	*old2_last;
	t_parser	*temp;

	old1_next = node1->next;
	old2_next = node2->next;
	temp = node1;
	jump_to_start(&temp);
	old1_last = temp;
	old2_last = temp;
	while (old1_last->next != node1)
		old1_last = old1_last->next;
	while (old2_last->next != node2)
		old2_last = old2_last->next;
	old1_last->next = node2;
	old2_last->next = node1;
	temp = node1->next;
	node1->next = node2->next;
	node2->next = temp;
}

// incase of leading redirs infront of command moves the comant infront of them
// otherwise this situation is bugged duo to parsing order (change needs huge refactor)
bool	move_commands_infront(t_parser *parser)
{
	t_parser	*last;

	while (parser->p_type != T_EOF)
	{
		last = last_parser(parser);
		while (parser->p_type == COMMAND && !is_operator(last->p_type) && last->p_type != T_EOF)
		{
			//TODO: figure out the diffrent syntax errors here (example: echo >aaa <sadad (echo) | >a <ad)
			if (!is_redir(last->p_type))
				return (print_error(true, NULL, NULL, type_to_str(parser->token->type)), false);
			swap_parsers(parser, last);
			last = last_parser(parser);
		}
		parser = parser->next;
	}
	return (true);
}

bool	new_merge_literals_start(t_parser *parser)
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
				rest_name_end = parser->rest_name;
				while (rest_name_end && rest_name_end->next)
					rest_name_end = rest_name_end->next;
				if (!rest_name_end)
				{
					parser->rest_name = parser->next;
					parser->next = parser->next->next;
					rest_name_end = parser->rest_name;
				}
				else
				{
					rest_name_end->next = parser->next;
					parser->next = parser->next->next;
					rest_name_end = rest_name_end->next;
				}
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
	if (!new_merge_literals_start(parser))
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
