/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 15:40:37 by frapp             #+#    #+#             */
/*   Updated: 2024/03/06 09:53:04 by frapp            ###   ########.fr       */
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
//void	print_parser_testing(t_parser *head);


int main(int ac, char *av[])
{
	printf("sizeof t_ast: %lu\n", sizeof(t_ast));
	printf("sizeof t_token: %lu\n", sizeof(t_token));
	printf("sizeof t_parser: %lu\n", sizeof(t_parser));
	printf("sizeof t_token_list: %lu\n", sizeof(t_token_list));
	printf("sizeof t_redir: %lu\n", sizeof(t_redir));
	return 0;
}
