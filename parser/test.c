/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 09:19:39 by frapp             #+#    #+#             */
/*   Updated: 2024/01/25 23:02:29 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	print_parser(t_parser *parser, int tree_level)
{
	// Print the circular linked list
	t_parser *current = parser;
	//int i = 0;
	do
	{
		if (current->p_type == T_EOF)
			break ;
		//print_indent(tree_level);
		//printf("lexer nb %d: \n", i++);
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
		token_node = token_node->next;
	}
}

void	print_arg_list(t_arg *arg, int level)
{
	while (arg)
	{
		print_indent(level);
		print_colored(token_type_to_string(arg->type), level);
		print_colored(" ; Name : ", level);
		print_token_list(arg->name, level);
		printf("\n");
		arg = arg->next;
	}
}

void	print_ast(t_ast *ast, int level, char *path)
{
	t_token_list	*token_node;

	print_colored("\nlevel: ", level);
	print_colored(ft_itoa(level), level);
	print_colored(" ; path: ", level);
	print_colored(path, level);
	printf("\n");
	print_indent(level);
	print_colored(token_type_to_string(ast->type), level);
	print_colored("; ", level);
	if (ast->name)
	{
		print_colored("Name: ", level);
		token_node = ast->name;
		print_token_list(token_node, level);
		printf("\n");
	}
	if (ast->redir_in)
	{
		print_indent(level);
		print_colored("Redir in: \n", level);
		print_arg_list(ast->redir_in, level);
	}
	if (ast->redir_out)
	{
		print_indent(level);
		print_colored("Redout in: \n", level);
		print_arg_list(ast->redir_out, level);
	}
	if (ast->arg)
	{
		print_indent(level);
		print_colored("Args: \n", level);
		print_arg_list(ast->arg, level);
	}
	// if (ast->val)
	// 	print_parser(ast->val, level);
	if (ast->left)
		print_ast(ast->left, level + 1, ft_strjoin(path, "->left"));
	if (ast->right)
		print_ast(ast->right, level + 1, ft_strjoin(path, "->right"));
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
	print_ast(ast, 0, ft_strdup("root"));
	return 0;
}


int main(int ac, char **av)
{
	a(ac, av);
	//system("leaks test");
	return (0);
}
