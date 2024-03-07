/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 21:11:04 by frapp             #+#    #+#             */
/*   Updated: 2024/03/07 09:09:03 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"
#include "internals_parser.h"
#include "../headers/lexer.h"

t_parser	*find_highest_operator(t_parser *parser)
{
	while (!is_command_block_terminator(parser->p_type)
		|| parser->p_type == PIPE)
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

t_parser	*unlink_left_get_left(t_parser *cut_location)
{
	t_parser	*left_head;
	t_parser	*left_last;
	t_parser	*temp;
	t_token		*left_eof_token;

	left_head = cut_location;
	jump_to_start(&left_head);
	if (left_head == cut_location)
		return (print_error(true, NULL, NULL,
				type_to_str(cut_location->p_type)), set_last_exit(2), NULL);
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

t_parser	*unlink_right_get_right(t_parser *cut_location)
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

// does not clean up the parser
// uses tokens of parser -->> dont free tokens when freeing parser
t_token_list	*extract_token_list(t_parser *parser, char name_or_arg)
{
	t_token_list	*new_list;

	if (!parser || parser->p_type == T_EOF || !parser->token || parser->token->type == T_EOF)
		return (NULL);
	new_list = ft_calloc(1, sizeof(t_token_list));
	if (!new_list)
		return (NULL);
	new_list->token = parser->token;
	if (name_or_arg == NAME)
	{
		if (!parser->rest_name)
			return (new_list);
		new_list->next = extract_token_list(parser->rest_name, RECURSIVE_CALL);
		if (!new_list->next)
			return (free(new_list), NULL);
	}
	else
	{
		if (!parser->next)
			return (new_list);
		new_list->next = extract_token_list(parser->next, RECURSIVE_CALL);
		if (!new_list->next)
			return (free(new_list), NULL);
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
			return (NULL);
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
	{
		cur->name = extract_token_list(parser->arg, NAME);
		if (!cur->name)
		{
		}
	}
	else
	{
		cur->name = ft_calloc(1, sizeof(t_arg));
		if (!cur->name)
		{
		}
		cur->name->token = parser->token;
		cur->name->next = extract_token_list(parser->rest_name, RECURSIVE_CALL);
		if (!cur->name->next)
		{
		}
	}
	cur->type = parser->token->type;
	return (head_arg);
}

t_result	append_redir(t_ast *ast_node, t_parser *args, t_redir **cur_redir)
{
	if (*cur_redir)
	{
		(*cur_redir)->next = ft_calloc(1, sizeof(t_redir));
		if (!(*cur_redir)->next)
			return (ERROR);
		*cur_redir = (*cur_redir)->next;
	}
	else
	{
		*cur_redir = ft_calloc(1, sizeof(t_redir));
		if (!*cur_redir)
			return (ERROR);
		ast_node->redir = (*cur_redir);
	}
	(*cur_redir)->type = args->token->type;
	(*cur_redir)->arg = append_arg(args->arg, (*cur_redir)->arg, true);
	if ((*cur_redir)->arg)
	{
		//malloc error
	}
	if (args->token->left_redir_arg != NULL)
		(*cur_redir)->left_redir_arg = ft_atoi(args->token->left_redir_arg);
	else
		(*cur_redir)->left_redir_arg = INIT_VAL;
	return (SUCCESS);
}

// Initializes a pipe and captures input until a termination string is encountered.
// Stores the read-end file descriptor as a string in the redir structure for later use.
// Writes each line of input to the write-end of the pipe, excluding the termination line.
t_result	parser_resovle_here_doc(t_redir *redir)
{
	int				pipe_fd[2];
	char			*termination;
	char			*line;
	char			*temp;

	termination = ft_strjoin(redir->arg->name->token->str_data, "\n");
	printf("termination: %s", termination);
	if (!termination)
		return (ERROR);
	//(void)redir;
	if (pipe(pipe_fd) == -1)
		return (free(termination), ERROR);
	free(redir->arg->name->token->str_data);
	redir->arg->name->token->str_data = ft_itoa(pipe_fd[READ]);
	temp = NULL;
	if (redir->arg->name->token->str_data)
		temp = ft_strjoin("<<<<", redir->arg->name->token->str_data);
	free(redir->arg->name->token->str_data);
	redir->arg->name->token->str_data = temp;
	if (!redir->arg->name->token->str_data)
	{
		close(pipe_fd[READ]);
		close(pipe_fd[WRITE]);
		free(termination);
		return (ERROR);
	}
	line = get_next_line(0);
	while (line)
	{
		if (ft_strcmp(line, termination) == 0)
		{
			my_free((void **)&line);
			break ;
		}
		ft_fprintf(pipe_fd[WRITE], "%s", line);
		my_free((void **)&line);
		line = get_next_line(0);
	}
	close(pipe_fd[WRITE]);
	return (SUCCESS);
}

t_ast	*build_leaf_node(t_ast *ast_node, t_parser *parser)
{
	t_redir					*cur_redir;
	t_parser				*args;

	ast_node->type = parser->p_type;
	ast_node->name = extract_token_list(parser, NAME);
	if (!ast_node->name)
	{
	}
	args = parser->arg;
	cur_redir = NULL;
	while (args)
	{
		if (is_redir(args->token->type))
		{
			// if (args->p_type != HERE_DOC && append_redir(ast_node, args, &cur_redir) == ERROR)
			// 	return (free_parser_main(parser, true),
			if (append_redir(ast_node, args, &cur_redir) == ERROR)
				return (free_parser_main(parser, true),
					free_ast(ast_node), NULL);
			if (cur_redir->type == HERE_DOC)
			{
				if (parser_resovle_here_doc(cur_redir) == ERROR)
				{//todo error
				}
			}
		}
		else if (args->p_type == ARGUMENT)
		{
			ast_node->arg = append_arg(args, ast_node->arg, true);
			if (!ast_node->arg)
				return (free_parser_main(parser, true),
					free_ast(ast_node), NULL);
		}
		else
		{
			print_error(true, "dubg (should not show)",
				"parser build_leaf_node", type_to_str(args->token->type));
			set_last_exit(2);
			return (free_parser_main(parser, true), free_ast(ast_node), NULL);
		}
		args = args->next;
	}
	free_parser_main(parser, false);
	return (ast_node);
}

t_result	build_operator_node(t_ast *ast_node, t_parser *highest_operator)
{
	t_left_right_parsers	child_parsers;

	ast_node->type = highest_operator->p_type;
	child_parsers = split_parser(highest_operator);
	if (!child_parsers.left || !child_parsers.right)
		return (ERROR);
	ast_node->left = build_ast(child_parsers.left);
	if (!ast_node->left)
		return (free_parser_main(highest_operator, true),
			free_ast(ast_node), free_parser_main(child_parsers.right, true), ERROR);
	ast_node->right = build_ast(child_parsers.right);
	if (!ast_node->right)
		return (free_parser_main(highest_operator, true),
			free_ast(ast_node), ERROR);
	free_parser_main(highest_operator, true);
	return (SUCCESS);
}

// TODO: handle errors correctly
t_ast *build_ast(t_parser *parser)
{
	t_parser				*highest_operator;
	
	t_ast					*ast_node;

	ast_node = ft_calloc(1, sizeof(t_ast));
	if (!ast_node)
		return (free_parser_main(parser, true), NULL);
	// if (parser->p_type != COMMAND)
	// {
	// 	return (print_error(true, "debug build_ast", NULL, type_to_str(parser->token->type)),
	// 		free_parser_main(parser, true), NULL);
	// }
	highest_operator = find_highest_operator(parser);
	if (!highest_operator)
		return (build_leaf_node(ast_node, parser));
	if (build_operator_node(ast_node, highest_operator) == ERROR)
		return (NULL);
	return (ast_node);
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

// Iterates through and frees all associated redirection and argument structures.
// If the here-doc arg starts with "<<<<" that means the following integer string
// corresponds to an open fd
void	free_redir(t_redir *redir)
{
	t_redir	*last;

	while (redir)
	{
		if (redir->type == HERE_DOC)
		{
			if (redir->arg->name->token->str_data
				&& !ft_strncmp(redir->arg->name->token->str_data,
					"<<<<", 4))
			{
				close(ft_atoi(redir->arg->name->token->str_data + 4));
			}
		}
		free_arg_list(redir->arg);
		last = redir;
		redir = redir->next;
		free(last);
	}
}

void	free_ast(t_ast *ast)
{
	if (ast->left)
		free_ast(ast->left);
	if (ast->right)
		free_ast(ast->right);
	free_token_list(ast->name);
	free_arg_list(ast->arg);
	free_redir(ast->redir);
	free(ast);
}
