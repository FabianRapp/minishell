/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:44:23 by frapp             #+#    #+#             */
/*   Updated: 2024/01/21 10:38:17 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/lexer.h"

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		printf("enter 1 string\n");
		return (0);
	}
	test_lexer_manualy(av[1]);
	return (0);
}
