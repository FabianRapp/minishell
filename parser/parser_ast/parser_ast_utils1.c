/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_utils1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 05:44:50 by frapp             #+#    #+#             */
/*   Updated: 2024/03/26 22:36:48 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_parser	*find_highest_operator(t_parser *parser)
{
	if (!parser)
		return (NULL);
	while (parser->p_type != T_EOF && parser->p_type != SEMICOL)
		parser = parser->next;
	if (parser->p_type == T_EOF)
		parser = parser->next;
	else
		return (parser);
	while (!is_command_block_terminator(parser->p_type)
		|| parser->p_type == PIPE)
	{
		parser = parser->next;
	}
	if (parser->p_type == T_EOF)
		parser = parser->next;
	else
		return (parser);
	while (parser->p_type != PIPE && parser->p_type != T_EOF)
		parser = parser->next;
	if (parser->p_type == T_EOF)
		return (NULL);
	return (parser);
}

static t_parser	*unlink_left_get_left(t_parser *cut_location)
{
	t_parser	*left_head;
	t_parser	*left_last;
	t_parser	*temp;
	t_token		*left_eof_token;

	left_head = cut_location;
	jump_to_start(&left_head);
	if (left_head == cut_location)
		return (print_error(true, NULL, NULL,
				type_to_error(cut_location->p_type)), set_last_exit(2), NULL);
	left_last = last_parser(cut_location);
	left_eof_token = ft_calloc(1, sizeof(t_token));
	if (!left_eof_token)
		return (NULL);
	left_eof_token->type = T_EOF;
	if (!insert_token(&left_last, left_eof_token))
		return (free_token(left_eof_token), NULL);
	left_last->next = left_head;
	temp = cut_location;
	while (temp->next && temp->p_type != T_EOF)
	{
		temp = temp->next;
	}
	temp->next = cut_location;
	return (left_head);
}

static t_parser	*unlink_right_get_right(t_parser *cut_location)
{
	t_parser	*right_head;
	t_parser	*right_end;
	t_token		*eof_token;
	t_parser	*temp;

	right_head = cut_location->next;
	if (right_head == cut_location || cut_location->p_type == T_EOF)
		return (NULL);
	right_end = right_head;
	while (right_end->p_type != T_EOF)
		right_end = right_end->next;
	right_end->next = right_head;
	eof_token = ft_calloc(1, sizeof(t_token));
	temp = cut_location;
	if (!eof_token || !insert_token(&cut_location, eof_token))
	{
		right_end->next = temp;
		return (NULL);
	}
	cut_location = temp;
	eof_token->type = T_EOF;
	cut_location->next->next = cut_location;
	return (right_head);
}

t_left_right_parsers	split_parser(t_parser *split_location)
{
	t_left_right_parsers	new_parsers;

	if (split_location->next == split_location
		|| (split_location->next->p_type == T_EOF
			&& split_location->next->next == split_location))
	{
		new_parsers.left = NULL;
		new_parsers.right = NULL;
	}
	else
	{
		new_parsers.left = unlink_left_get_left(split_location);
		new_parsers.right = unlink_right_get_right(split_location);
	}
	if (new_parsers.left == NULL || new_parsers.right == NULL)
	{
		free_parser_main(new_parsers.left, true);
		free_parser_main(new_parsers.right, true);
		free_parser_main(split_location, true);
		new_parsers.left = NULL;
		new_parsers.right = NULL;
	}
	return (new_parsers);
}

t_token_list	*extract_token_list(t_parser *parser, char name_or_arg)
{
	t_token_list	*new_list;

	if (!parser || !parser->token || parser->token->type == T_EOF)
		return (NULL);
	new_list = ft_calloc(1, sizeof(t_token_list));
	if (!new_list)
		return (NULL);
	new_list->token = parser->token;
	parser->token = NULL;
	if (name_or_arg == NAME)
	{
		if (!parser->rest_name)
			return (new_list);
		new_list->next = extract_token_list(parser->rest_name, RECURSIVE_CALL);
	}
	else
	{
		if (!parser->next)
			return (new_list);
		new_list->next = extract_token_list(parser->next, RECURSIVE_CALL);
	}
	if (!new_list->next)
		return (free(new_list), NULL);
	return (new_list);
}
