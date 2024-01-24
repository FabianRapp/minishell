/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugging.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 07:01:13 by frapp             #+#    #+#             */
/*   Updated: 2024/01/24 21:16:46 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/lexer.h"
#include "../headers/parser.h"

const char* token_type_to_string(t_type tokenType)
{
	switch (tokenType)
	{
		case UNKNOWN: return "UNKNOWN";
		case T_EOF: return "T_EOF";
		case WHITE_SPACE: return "WHITE_SPACE";
		case PIPE: return "PIPE";
		case OR: return "OR";
		case AND: return "AND";
		case ENV_VAR: return "ENV_VAR";
		case EXIT_STATUS_REQUEST: return "EXIT_STATUS_REQUEST";
		case FT_BUILDIN: return "FT_BUILDIN";
		case WILDCARD: return "WILDCARD";
		case CTRL_C: return "CTRL_C";
		case CTRL_D: return "CTRL_D";
		case CTRL_BACKSLASH: return "CTRL_BACKSLASH";
		case LITERAL: return "LITERAL";
		case INTERPRETED: return "INTERPRETED";
		case REDIR_IN: return "REDIR_IN";
		case REDIR_OUT: return "REDIR_OUT";
		case REDIR_APPEND: return "REDIR_APPEND";
		case HERE_DOC: return "HERE_DOC";
		case SUBSHELL: return "SUBSHELL";
		case FLAG: return "FLAG";
		case WORD: return "WORD";
		case COMMAND: return "COMMAND";
		case ARGUMENT: return "ARGUMENT";
		case REDIR_ARG: return "REDIR_ARG";
		default: return "Type not found";
	}
}

void print_indent(int depth)
{
	for (int i = 0; i < depth; i++) {
		printf("  ");
	}
}

void print_parser_tree(t_parser *parser, const char *label, int depth)
{
	if (parser) {
		if (!ft_strcmp(label, "arg"))
		{
			print_indent(depth);
			printf("%s:\n", label);
		}
		while (parser && parser->p_type != T_EOF) {
			print_token(parser->token, parser, depth + 1);
			parser = parser->next;
		}
		print_indent(depth);
		printf("--end %s\n", label);
	}
}

void	print_token(t_token *token, t_parser *parser, int depth)
{
	const char* token_type_str = token_type_to_string(token->type);

	print_indent(depth);
	if (!parser)
		printf("%s", token_type_str);
	else
	{
		const char* parser_type_str = token_type_to_string(parser->p_type);
		printf("%s/%s: ", parser_type_str, token_type_str);
		if (parser->token->str_data)
			printf("%s", parser->token->str_data);
		if (parser->token->unknown)
			printf("unknown %s", parser->token->str_data);
		if (parser->rest_name) {
			print_parser_tree(parser->rest_name, "name", depth + 1);
		}
		if (parser->arg) {
			print_parser_tree(parser->arg, "arg", depth + 1);
		}
	}
}
// 		if (parser->arg)
// 			printf("\narg\n");
// 		if (parser->rest_name)
// 			printf("\nrest name \n");
// 		if (parser->arg && (token->type == REDIR_IN || token->type == REDIR_OUT || token->type == REDIR_APPEND || token->type == HERE_DOC))
// 		{
// 			t_parser	*list;
// 			printf("\n--start redir data:\n");
// 			list = (t_parser *)(parser->arg);
// 			while (list && list->p_type != T_EOF)
// 			{
// 				print_token(list->token, NULL);
// 				printf("\n");
// 				list = list->next;
// 			}
// 			printf("\n--end redir data data\n");
// 		}
// 		if (parser->arg && (parser->p_type == COMMAND))
// 		{
// 			t_parser	*list;
// 			printf("\n--start command data:\n");
// 			list = (t_parser *)(parser->arg);
// 			while (list && list->p_type != T_EOF)
// 			{
// 				print_token(list->token, NULL);
// 				printf("\n");
// 				list = list->next;
// 			}
// 			printf("\n--end command data data\n");
// 		}
// 		if (parser->rest_name && (parser->p_type == WORD || parser->p_type == COMMAND || parser->p_type == WORD))
// 		{
// 			t_parser	*list;
// 			printf("\n--start rest_name:\n");
// 			list = (t_parser *)(parser->rest_name);
// 			while (list && list->p_type != T_EOF)
// 			{
// 				print_token(list->token, NULL);
// 				printf("\n");
// 				list = list->next;
// 			}
// 			printf("\n--end rest_name\n");
// 		}
// 	}
// }

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
		print_token(token, NULL, 0);
		printf("\n");
		token = next_new_token(&lexer);
	}
	printf("\t");
	print_token(token, NULL, 0);
	return (true);
}
