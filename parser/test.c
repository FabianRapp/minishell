/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 09:19:39 by frapp             #+#    #+#             */
/*   Updated: 2024/01/26 04:04:09 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"



int a(int ac, char **av)
{
	t_ast	*ast;

	if (ac != 2)
	{
		printf("wrong ac!\n");
		return (0);
	}
	ast = parser(av[1]);
	if (!ast)
	{
		printf("not ast\n");
		return (0);
	}
	print_ast(ast);
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
