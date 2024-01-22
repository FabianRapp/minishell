/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugging.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 07:01:13 by frapp             #+#    #+#             */
/*   Updated: 2024/01/22 21:18:34 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/lexer.h"

void	print_token(t_token token)
{
	printf("token input pos: %d\t", token.input_position);
	if (token.type == UNKNOWN) printf("UNKNOWN %c", token.unknown);
	else if (token.type == T_EOF) printf("T_EOF");
	else if (token.type == WHITE_SPACE) printf("WHITE_SPACE");
	else if (token.type == WORD) printf("WORD  : %s", token.str_data);
	else if (token.type == ENV_VAR) printf("ENV_VAR  : %s", token.str_data);
	else if (token.type == EXIT_STATUS_REQUEST) printf("EXIT_STATUS_REQUEST");
	else if (token.type == FT_BUILDIN) printf("FT_BUILDIN %s", token.str_data);
	else if (token.type == PIPE) printf("PIPE");
	else if (token.type == OR) printf("OR");
	else if (token.type == AND) printf("AND");
	else if (token.type == WILDCARD) printf("WILDCARD");
	else if (token.type == CTRL_C) printf("CTRL_C");
	else if (token.type == CTRL_D) printf("CTRL_D");
	else if (token.type == CTRL_BACKSLASH) printf("CTRL_BACKSLASH");
	else if (token.type == LITERAL) printf("LITERAL: %s", token.str_data);
	else if (token.type == INTERPRETED) printf("INTERPRETED: %s", token.str_data);
	else if (token.type == SUBSHELL) printf("SUBSHELL: %s", token.str_data);
	else if (token.type == FLAG) printf("FLAG: %s", token.str_data);
	else if (token.type == REDIR_IN) printf("REDIR_IN");
	else if (token.type == REDIR_OUT) printf("REDIR_OUT");
	else if (token.type == REDIR_APPEND) printf("REDIR_APPEND");
	else if (token.type == HERE_DOC) printf("HERE_DOC");
	else if (token.type == REDIR_ARG) printf("REDIR_ARG: %s", token.str_data);
	else printf("Type not found");
}

bool	test_lexer_manualy(char *str)
{
	t_lexer			lexer;
	t_token			*token;

	printf("test str: \"%s\"\n", str);
	printf("lexer output before in list:\n");
	lexer = new_lexer(str);
	token = next_new_token(&lexer);
	while (token->type != T_EOF)
	{
		printf("\t");
		print_token(*token);
		printf("\n");
		token = next_new_token(&lexer);
	}
	printf("\t");
	print_token(*token);
	return (true);
}
