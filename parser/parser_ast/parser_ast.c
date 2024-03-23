/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 06:15:18 by frapp             #+#    #+#             */
/*   Updated: 2024/03/23 15:59:24 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/parser.h"
#include "../internals_parser.h"
#include "../../headers/lexer.h"

t_ast	*append_redirs_args(t_parser *args, t_ast *ast_node, t_parser *parser)
{
	t_redir		*cur_redir;

	cur_redir = NULL;
	while (args)
	{
		if (is_redir(args->token->type))
		{
			if ((append_redir(ast_node, args, &cur_redir) == ERROR)
				|| (cur_redir->type == HERE_DOC && (parser_resovle_here_doc
						(cur_redir) == ERROR)) || (cur_redir->type == HERE_STR
					&& (parser_resovle_here_str(cur_redir) == ERROR)))
				return (free_parser_main(parser, true), free_ast(ast_node),
					NULL);
		}
		else if (args->p_type == ARGUMENT)
		{
			ast_node->arg = append_arg(args, ast_node->arg);
			if (!ast_node->arg)
				return (free_parser_main(parser, true),
					free_ast(ast_node), NULL);
		}
		args = args->next;
	}
	return (free_parser_main(parser, false), ast_node);
}

t_ast	*build_leaf_node(t_ast *ast_node, t_parser *parser)
{
	ast_node->type = parser->p_type;
	ast_node->name = extract_token_list(parser, NAME);
	if (!ast_node->name)
		return (free_parser_main(parser, true), free_ast(ast_node), NULL);
	return (append_redirs_args(parser->arg, ast_node, parser));
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
			free_ast(ast_node), free_parser_main(child_parsers.right, true),
			ERROR);
	ast_node->right = build_ast(child_parsers.right);
	if (!ast_node->right)
		return (free_parser_main(highest_operator, true),
			free_ast(ast_node), ERROR);
	free_parser_main(highest_operator, true);
	return (SUCCESS);
}

t_ast	*build_ast(t_parser *parser)
{
	t_parser				*highest_operator;
	t_ast					*ast_node;

	ast_node = ft_calloc(1, sizeof(t_ast));
	if (!ast_node)
		return (free_parser_main(parser, true), NULL);
	highest_operator = find_highest_operator(parser);
	if (!highest_operator)
		return (build_leaf_node(ast_node, parser));
	if (build_operator_node(ast_node, highest_operator) == ERROR)
		return (NULL);
	return (ast_node);
}
