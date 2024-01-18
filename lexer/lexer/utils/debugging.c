/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugging.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 07:01:13 by frapp             #+#    #+#             */
/*   Updated: 2024/01/18 11:33:36 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/lexer.h"

void	print_token(t_token token)
{
	if (token.type == UNKNOWN) printf("UNKNOWN %c", token.unknown);
	else if (token.type == T_EOF) printf("T_EOF");
	else if (token.type == WHITE_SPACE) printf("WHITE_SPACE");
	else if (token.type == WORD) printf("WORD  : %s", token.str);
	else if (token.type == ENV_VAR) printf("ENV_VAR  : %s", token.str);
	else if (token.type == EXIT_STATUS_REQUEST) printf("EXIT_STATUS_REQUEST");
	else if (token.type == FT_BUILDIN) printf("FT_BUILDIN %s", token.str);
	else if (token.type == PIPE) printf("PIPE");
	else if (token.type == OR) printf("OR");
	else if (token.type == AND) printf("AND");
	else if (token.type == WILDCARD) printf("WILDCARD");
	else if (token.type == CTRL_C) printf("CTRL_C");
	else if (token.type == CTRL_D) printf("CTRL_D");
	else if (token.type == CTRL_BACKSLASH) printf("CTRL_BACKSLASH");
	//else if (token.type == INTEGER) printf("INTEGER: %lld", token.int_val);
	else if (token.type == LITERAL) printf("LITERAL: %s", token.str);
	else if (token.type == INTERPRETED) printf("INTERPRETED: %s", token.str);
	else if (token.type == REDIR) printf("REDIR: %lld", token.int_val);
	else if (token.type == SUBSHELL) printf("SUBSHELL: %s", token.str);
	else if (token.type == FLAG) printf("FLAG: %s", token.str);
	else printf("Type not found");
}

void	print_list(t_token *first)
{
	t_token	*cur;

	cur = first;
	while (cur)
	{
		if (!(!cur || cur->next || !(cur->last)))
			printf("error, list does not loop!\n");
		printf("\t");
		print_token(*cur);
		printf("\n");
		cur = cur->next;
		if (cur == first)
			break ;
	}
	if (!(!cur || cur->next || !(cur->last)))
		printf("error, list does not loop!\n");
}

bool	test_lexer_manualy(char *str)
{
	t_lexer			lexer;
	t_token			token;
	t_token			*list;

	printf("test str: \"%s\"\n", str);
	printf("lexer output before in list:\n");
	lexer = new_lexer(str);
	token = next_new_token(&lexer);
	while (token.type != T_EOF)
	{
		printf("\t");
		print_token(token);
		printf("\n");
		token = next_new_token(&lexer);
	}
	printf("\t");
	print_token(token);
	printf("\n\n");
	printf("token list looped once to the start:\n");
	list = token_list(str);
	print_list(list);
	return (true);
}
