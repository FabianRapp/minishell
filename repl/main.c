/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:00:27 by frapp             #+#    #+#             */
/*   Updated: 2024/01/26 04:11:56 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parser.h"



int	main(void)
{
	t_ast	*ast;
	char	*input;

	input = readline("minishell: ");
	while (input != NULL)
	{
		if (*input)
		{
			add_history(input);
			ast = parser(input);
			print_ast(ast);
		}
		free(input);
		input = readline("minishell: ");
	}
	return (0);
}
