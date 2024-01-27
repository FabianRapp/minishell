/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugging.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 07:01:13 by frapp             #+#    #+#             */
/*   Updated: 2024/01/27 20:28:16 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/lexer.h"
#include "../headers/parser.h"


void print_colored(const char *text, int color_index)
{
	char *colors[] = {RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE};
	int num_colors = sizeof(colors) / sizeof(colors[0]);
	char *color = colors[color_index % num_colors];
	printf("%s%s%s", color, text, RESET_COLOR);
}

const char	*token_type_to_string(t_type tokenType)
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
		//case FT_BUILDIN: return "FT_BUILDIN";
		case WILDCARD: return "WILDCARD";
		//case CTRL_C: return "CTRL_C";
		//case CTRL_D: return "CTRL_D";
		//case CTRL_BACKSLASH: return "CTRL_BACKSLASH";
		case WORD: return "WORD";
		case INTERPRETED: return "INTERPRETED";
		case REDIR_IN: return "REDIR_IN";
		case REDIR_OUT: return "REDIR_OUT";
		case REDIR_APPEND: return "REDIR_APPEND";
		case HERE_DOC: return "HERE_DOC";
		case SUBSHELL: return "SUBSHELL";
		//case FLAG: return "FLAG";
		case COMMAND: return "COMMAND";
		case ARGUMENT: return "ARGUMENT";
		case REDIR_ARG: return "REDIR_ARG";
		case VOID: return "VOID";
		case DUMMY_COMMAND: return "DUMMY_COMMAND";
		case LITERAL: return "LITERAL";
		default: return "Type not found";
	}
}

void	print_indent(int depth, bool left)
{
	if (!left)
	{
		for (int i = 0; i < depth; i++) {
			print_colored(" ", depth);
		}
	}
	else
	{
		for (int i = 0; i < depth - 1; i++) {
			print_colored(" ", depth);
		}
		print_colored("│", depth - 1);
	}
	print_colored("│", depth);
}

void	print_indent_arg(int depth)
{
	for (int i = 0; i < depth - 1; i++) {
		print_colored(" ", depth);
	}
	print_colored("├", depth - 1);
	print_colored("┐", depth);
}

//├ ─ ─ │  └──
void	print_new_indent(int depth, bool left)
{

	//print_indent(depth);
	//printf("\n");
	for (int i = 0; i < depth - 1; i++) {
		print_colored(" ", depth);
		//print_colored("─", depth);
	}
	if (depth > 0 && !left)
	{
		print_colored("└", depth - 1);
		print_colored("├─", depth);
	}
	else if (depth > 0 && left)
	{
		print_colored("├", depth - 1);
		print_colored("┬─", depth);
	}
	else
		print_colored("│", depth);
}


void print_parser_tree(t_parser *parser, const char *label, int depth)
{
	if (parser) {
		while (parser && parser->p_type != T_EOF) {
			print_token(parser->token, parser, depth);
			parser = parser->next;
		}
		(void)label;
		// print_indent(depth);
		// print_colored("end ", depth);
		// print_colored(label, depth);
		// print_colored("\n", depth);
	}
}

void	print_token(t_token *token, t_parser *parser, int depth)
{
	const char* token_type_str = token_type_to_string(token->type);

	printf("\n");
	print_indent(depth, false);
	if (!parser)
	{
		print_colored(token_type_str, depth);
		if (token->str_data)
			print_colored(token->str_data, depth);
		if (token->unknown)
			print_colored("unknown", depth); //printf("unknown %s", parser->token->str_data);
	}
	else
	{
		const char* parser_type_str = token_type_to_string(parser->p_type);
		print_colored(parser_type_str, depth);
		print_colored("/", depth);
		print_colored(token_type_str, depth);
		print_colored(" ", depth);

		if (parser->rest_name) {
		//	print_parser_tree(parser->rest_name, "name", depth + 1);
		}
		if (parser->arg) {
			printf("\n");
			print_indent(depth, false);
			if (parser->arg->p_type == ARGUMENT)
				print_colored("Command arguments:", depth);
			else if (parser->arg->p_type == REDIR_ARG)
				print_colored("Redir argument:\n", depth);
			else
				print_colored("why is this here:\n", depth);
			print_parser_tree(parser->arg, "arg", depth);
		}
	}
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
		print_token(token, NULL, 0);
		printf("\n");
		token = next_new_token(&lexer);
	}
	printf("\t");
	print_token(token, NULL, 0);
	return (true);
}
