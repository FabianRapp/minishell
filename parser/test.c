/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 09:19:39 by frapp             #+#    #+#             */
/*   Updated: 2024/01/24 21:30:21 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

#define TEST_MAIN 1
#ifdef TEST_MAIN

int a(int ac, char **av)
{
	if (ac != 2)
	{
		printf("wrong ac!\n");
		return (0);
	}
	t_parser *head = parser(av[1]);
	if (head == NULL)
	{
		printf("Parser returned NULL.\n");
		return 1;
	}
	// Print the circular linked list
	t_parser *current = head;
	do
	{
		print_token(current->token, current, 0);
		printf("\n");
		current = current->next;
	} while (current != head && current);
	if (!current)
		printf("parser object 1 or 0 nodes (should be 1)\n");
	free_parser_main(current);
	return 0;
}



int main(int ac, char **av)
{
	a(ac, av);
	//system("leaks test");
	return (0);
}


#endif
