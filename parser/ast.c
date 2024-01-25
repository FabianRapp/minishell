/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 21:11:04 by frapp             #+#    #+#             */
/*   Updated: 2024/01/25 21:51:34 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"
#include "internals_parser.h"
#include "../headers/lexer.h"

t_parser	*last_parser(t_parser *parser)
{
	t_parser	*last;

	if (!parser)
	{
		printf("bug shows in last_parser\n");
		return (NULL);
	}
	last = parser;
	while (last->next != parser)
		last = last->next;
	return (last);
}

t_parser	*find_highest_operator(t_parser *parser)
{
	if (!parser)
	{
		printf("bug shows in last_parser\n");
		return (NULL);
	}
	while (parser->p_type != AND && parser->p_type != OR && parser->p_type != T_EOF)
		parser = parser->next;
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

// remove the list after cut_locataion and returns it as a seperated cirular linked list
// cut_location gets removed from both lists, so keep a poiter to the first if it exists
// does nothing and returns NULL if the given list only contains 1 element or the cut_location is T_EOF
// adds a new T_EOF token to the old list if it got removed
t_parser	*remove_back(t_parser *cut_location)
{
	t_parser	*right_head;
	t_parser	*right_end;
	t_parser	*left_head;
	t_parser	*left_last;
	t_token		*left_eof_token;

	right_head = cut_location->next;
	if (right_head == cut_location || cut_location->p_type == T_EOF)
	{
		return (NULL);
	}
	left_head = cut_location;
	jump_to_start(&left_head);
	left_last = last_parser(cut_location);
	if (left_head != cut_location)
	{
		left_eof_token = ft_calloc(1, sizeof(t_token));
		if (!left_eof_token)
			return (cleanup(), NULL);
		left_eof_token->type = T_EOF;
		insert_token(&left_last, left_eof_token);
		left_last->next = left_head;
	}
	right_end = right_head;
	while (right_end->p_type != T_EOF)
		right_end = right_end->next;
	right_end->next = right_head;
	return (right_head);
}

t_left_right_parsers	split_parser(t_parser *split_location)
{
	t_left_right_parsers	new_parsers;

	if (split_location->next == split_location 
		|| (split_location->next->p_type == T_EOF && split_location->next->next == split_location))
		new_parsers.left = NULL;
	else
	{
		new_parsers.left = split_location;
		jump_to_start(&(new_parsers.left));
	}
	new_parsers.right = remove_back(split_location);
	return (new_parsers);
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

t_ast *build_ast(t_parser *parser)
{
	t_parser				*highest_operator;
	t_left_right_parsers	child_parsers;
	t_ast					*ast_node;

	ast_node = ft_calloc(1, sizeof(t_ast));
	if (!ast_node)
		return (cleanup(), NULL);
	highest_operator = find_highest_operator(parser);
	if (!highest_operator)//is leaf node
	{
		ast_node->val = parser;
		ast_node->type = parser->p_type;
		ast_node->name = extract_token_list(parser, NAME);
		
		return (ast_node);
	}
	ast_node->type = highest_operator->p_type;
	if (highest_operator->rest_name || highest_operator->arg)
	{
		printf("build ast wtf\n");
		exit(0);
	}
	child_parsers = split_parser(highest_operator);
	ast_node->left = build_ast(child_parsers.left);
	ast_node->right = build_ast(child_parsers.right);
	free_token(highest_operator->token);
	free(highest_operator);
	return (ast_node);
}
