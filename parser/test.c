/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 09:19:39 by frapp             #+#    #+#             */
/*   Updated: 2024/01/26 03:16:40 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	print_parser(t_parser *parser, int tree_level)
{
	t_parser *current = parser;
	do
	{
		if (current->p_type == T_EOF)
			break ;
		print_token(current->token, current, tree_level);
		current = current->next;
	} while (current != parser && current);
	//if (!current)
		//printf("parser object 1 or 0 nodes (should be 1)\n");
	// if (current->p_type == T_EOF)
	// {
	// 	print_token(current->token, current, tree_level);
	// 	printf("\n");
	// }
	//free_parser_main(parser);
}

void	print_token_list(t_token_list *token_node, int level)
{
	while (token_node)
	{
		print_colored(token_type_to_string(token_node->token->type), level);
		print_colored(": ", level);
		print_colored(token_node->token->str_data, level);
		print_colored(" ; ", level);
		printf("\n");
		token_node = token_node->next;
	}
}

void	print_arg_list(t_arg *arg, int level, bool left)
{
	while (arg)
	{
		print_indent(level, left);
		print_colored(token_type_to_string(arg->type), level);
		print_colored(" ; Name : ", level);
		print_token_list(arg->name, level);
		arg = arg->next;
	}
}

void	print_ast(t_ast *ast, int level, char *path, bool left)
{
	t_token_list	*token_node;

	print_new_indent(level, left);
	print_colored("level: ", level);
	char	*a = ft_itoa(level);
	print_colored(a, level);
	free(a);
	print_colored(" ; path: ", level);
	print_colored(path, level);
	printf("\n");
	print_indent(level, left);
	print_colored(token_type_to_string(ast->type), level);
	print_colored("; ", level);
	if (!(ast->name) && !(ast->redir_in) && !(ast->redir_out) && !(ast->arg))
		printf("\n");
	if (ast->name)
	{
		printf("\n");
		print_indent(level, left);
		print_colored("Name: ", level);
		token_node = ast->name;
		print_token_list(token_node, level);
	}
	if (ast->redir_in)
	{
		print_indent(level, left);
		print_colored("Redir in: \n", level);
		print_arg_list(ast->redir_in, level, left);
	}
	if (ast->redir_out)
	{
		print_indent(level, left);
		print_colored("Redout out: \n", level);
		print_arg_list(ast->redir_out, level, left);
	}
	if (ast->arg)
	{
		print_indent(level, left);
		print_colored("Args: \n", level);
		print_arg_list(ast->arg, level, left);
	}
	if (ast->left)
		print_ast(ast->left, level + 1, ft_strjoin(path, "->left"), true);
	if (ast->right)
		print_ast(ast->right, level + 1, ft_strjoin(path, "->right"), false);
	free (path);
}

int a(int ac, char **av)
{
	t_ast	*ast;

	if (ac != 2)
	{
		printf("wrong ac!\n");
		return (0);
	}
	ast = parser(av[1]);
	print_ast(ast, 0, ft_strdup("root"), false);
	free_ast(ast);
	return (0);
}

void print_type_sizes()
{
	printf("Size of t_operators_ast: %zu bytes\n", sizeof(t_operators_ast));
	printf("Size of t_parser: %zu bytes\n", sizeof(t_parser));
	printf("Size of t_token_list: %zu bytes\n", sizeof(t_token_list));
	printf("Size of t_arg: %zu bytes\n", sizeof(t_arg));
	printf("Size of t_ast: %zu bytes\n", sizeof(t_ast));
	printf("Size of t_left_right_parsers: %zu bytes\n", sizeof(t_left_right_parsers));
	printf("Size of t_type: %zu bytes\n", sizeof(t_type));
	printf("Size of t_token: %zu bytes\n", sizeof(t_token));
	printf("Size of t_lexer: %zu bytes\n", sizeof(t_lexer));
}

int main(int ac, char **av)
{
	//print_type_sizes();
	a(ac, av);
	//system("leaks test");
	return (0);
}
