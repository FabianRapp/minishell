/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 15:40:37 by frapp             #+#    #+#             */
/*   Updated: 2024/02/14 06:48:14 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minishell.h"
#include "parser/internals_parser.h"
#include "headers/parser.h"
/*
make libs
cc testing.c parser.a libft.a lexer.a  -fsanitize=address
./a.out
*/
void	print_parser_testing(t_parser *head);


int main(int ac, char *av[])
{
	t_parser *list;

	char	*input;

	if (ac > 1)
		input = av[1];
	else
		input = ft_strdup(">a echo >b");
	printf("%s\n", input);
	list = parser_testing(input);
	print_parser_testing(list);
	return 0;
}

void print_parser_testing(t_parser *head)
{
	t_parser *current;
	static int	count = 0;
	int	inter_count = 100;

	current = head;
	while (current != NULL && current->p_type != T_EOF)
	{
		if (++count > inter_count)
		{
			printf("inf loop (reached %d iterations)\n", inter_count);
			exit (1);
		}
		char	*str_data = current->token->str_data;
		printf("%s: %s\n", type_to_str_type(current->p_type), str_data);
		if (current->arg)
		{
			printf("args:\n");
			print_parser_testing(current->arg);
		}
		current = current->next;
		
		printf("--------------------------------------\n");
	}
}
