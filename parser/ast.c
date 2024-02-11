/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 21:11:04 by frapp             #+#    #+#             */
/*   Updated: 2024/02/11 00:53:35 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"
#include "internals_parser.h"
#include "../headers/lexer.h"

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
// cut_location gets removed from both lists: keep a poiter to the first if it exists
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
			return (cleanup("remove_back"), NULL);
		left_eof_token->type = T_EOF;
		if (!insert_token(&left_last, left_eof_token))
		{
			//TODO: left_lasts list is freed here/ idk what this code does atm, handle the situation
			return (NULL);
		}
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
		return (cleanup("extract_token_list"), NULL);
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

// returns the head of the token list, NULL on malloc fail
// assums the given list to be the head
t_arg	*append_arg(t_parser *parser, t_arg *head_arg, bool leading_node)
{
	t_arg	*cur;

	if (!parser)
		return (NULL);
	if (!head_arg) 
	{
		head_arg = ft_calloc(1, sizeof(t_arg));
		if (!head_arg)
			return (cleanup("extract_token_list"), NULL);
		cur = head_arg;
	}
	else
	{
		cur = head_arg;
		while (cur->next)
			cur = cur->next;
		cur->next = ft_calloc(1, sizeof(t_arg));
		cur = cur->next;
		if (!cur)
			return (cleanup("extract_token_list"), NULL);
	}
	if (!leading_node)
		cur->name = extract_token_list(parser->arg, NAME);
	else
	{
		cur->name = ft_calloc(1, sizeof(t_arg));
		cur->name->token = parser->token;
		if (!cur->name)
			return (cleanup("extract_token_list"), NULL);
		cur->name->next = extract_token_list(parser->rest_name, RECURSIVE_CALL);
	}
	cur->type = parser->token->type;
	return (head_arg);
}

t_ast	*build_leaf_node(t_ast *ast_node, t_parser *parser)
{
	t_redir					*cur_redir;
	t_parser				*args;

	ast_node->type = parser->p_type;
	ast_node->name = extract_token_list(parser, NAME);
	//free_ncircular_parser(parser->rest_name, false);
	args = parser->arg;
	cur_redir = NULL;
	while (args)
	{
		if (is_redir(args->token->type))
		{
			if (cur_redir)
			{
				cur_redir->next = ft_calloc(1, sizeof(t_redir));
				cur_redir = cur_redir->next;
				cur_redir->type = args->token->type;
				//printf("redir type: %s \n", type_to_str_type(cur_redir->type));
				cur_redir->arg = append_arg(args->arg, cur_redir->arg, true);
			}
			else
			{
				cur_redir = ft_calloc(1, sizeof(t_redir));
				if (!cur_redir)
				{
					//malloc fail
				}
				ast_node->redir = cur_redir;
				cur_redir->type = args->token->type;
				cur_redir->arg = append_arg(args->arg, cur_redir->arg, true);
				//printf("redir type: %s \n", type_to_str_type(cur_redir->type));
				// printf("redir args:\n");
				// print_arg_list(cur_redir->arg, 10, 0);
				// printf("\n");
			}
		}
		else if (args->p_type == ARGUMENT)
			ast_node->arg = append_arg(args, ast_node->arg, true);
		else
		{
			printf("build ast debug:\n");
			print_token(args->token, args, 2);
			//exit(0);
		}
		//system("leaks minishell");
		args = args->next;
	}
	// if ((last_parser(parser) && last_parser(parser)->token->type != T_EOF)
	// 		|| (parser->next && !is_operator(parser->next) && parser->token->type != T_EOF))
	//if (ast_node->type != REDIR_ARG)
		free_parser_main(parser, false);
	//system("leaks minishell");
	return (ast_node);
}

// TODO: handle errors and cleanup correctly
t_ast *build_ast(t_parser *parser)
{
	t_parser				*highest_operator;
	t_left_right_parsers	child_parsers;
	t_ast					*ast_node;

	ast_node = ft_calloc(3, sizeof(t_ast)); ///TODO why 3??
	if (!ast_node)
		return (free_parser_main(parser, true), NULL);
	//system("leaks minishell");
	if (parser->p_type != COMMAND)
		return (print_error(true, NULL, NULL, type_to_str(parser->token->type)),
			free_parser_main(parser, true), NULL);
	highest_operator = find_highest_operator(parser);
	if (!highest_operator)//is leaf node
		return (build_leaf_node(ast_node, parser));
	ast_node->type = highest_operator->p_type;
	if (highest_operator->rest_name || highest_operator->arg)
	{
		printf("debug: build ast wtf\n");
		exit(1);
	}
	child_parsers = split_parser(highest_operator);
	ast_node->left = build_ast(child_parsers.left);
	if (!ast_node->left)
	{// handle error
	}
	ast_node->right = build_ast(child_parsers.right);
	if (!ast_node->right)
	{// handle error
	}
	return (free_token(highest_operator->token), free(highest_operator), ast_node);
}

void	free_token_list(t_token_list *list)
{
	t_token_list	*last;

	while (list)
	{
		last = list;
		list = list->next;
		free_token(last->token);
		free(last);
	}
}

void	free_arg_list(t_arg *list)
{
	t_arg	*last;

	while (list)
	{
		last = list;
		list = list->next;
		free_token_list(last->name);
		free(last);
	}
}

//TODO: redir rework
void	free_ast(t_ast *ast)
{
	if (ast->left)
		free_ast(ast->left);
	if (ast->right)
		free_ast(ast->right);
	if (ast->fd[IN] != IN)
		close(ast->fd[IN]);
	if (ast->fd[OUT] != OUT)
		close (ast->fd[OUT]);
	free_token_list(ast->name);
	free_arg_list(ast->arg);
	free(ast);
}
