/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:44:23 by frapp             #+#    #+#             */
/*   Updated: 2024/01/17 08:06:04 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer_tester.h>

const char *type_to_string(t_type type)
{
	switch (type) {
		case UNKNOWN: return "UNKNOWN";
		case T_EOF: return "T_EOF";
		case WHITE_SPACE: return "WHITE_SPACE";
		case STRING: return "STRING";
		//case LESS_THAN: return "LESS_THAN";
		//case GREATER_THAN: return "GREATER_THAN";

		case EXIT_STATUS_REQUEST: return "EXIT_STATUS_REQUEST";
		case PATH: return "PATH";
		case FT_BUILDIN: return "FT_BUILDIN";
		case PATH_FUNCTION: return "PATH_FUNCTION";
		case PIPE: return "PIPE";
		case OR: return "OR";
		case AND: return "AND";
		case WILDCARD: return "WILDCARD";
		case CTRL_C: return "CTRL_C";
		case CTRL_D: return "CTRL_D";
		case CTRL_BACKSLASH: return "CTRL_BACKSLASH";
		case INTEGER: return "INTEGER";
		case LITERAL: return "LITERAL";
		case INTERPRETED: return "INTERPRETED";
		case REDIR: return "REDIR";
		case ENV_VAR: return "ENV_VAR";
		case SUBSHELL: return "SUBSHELL";
		default: return "INVALID_TYPE";
	}
}

void	print_token(t_token token)
{
	if (token.type == UNKNOWN) printf("UNKNOWN %c", token.unknown);
	else if (token.type == T_EOF) printf("T_EOF");
	else if (token.type == WHITE_SPACE) printf("WHITE_SPACE");
	else if (token.type == STRING) printf("STRING  : %s", token.str);
	else if (token.type == ENV_VAR) printf("ENV_VAR  : %s", token.str);
	else if (token.type == EXIT_STATUS_REQUEST) printf("EXIT_STATUS_REQUEST");
	else if (token.type == PATH) printf("PATH");
	else if (token.type == FT_BUILDIN) printf("FT_BUILDIN %s", token.str);
	else if (token.type == PATH_FUNCTION) printf("PATH_FUNCTION");
	else if (token.type == PIPE) printf("PIPE");
	else if (token.type == OR) printf("OR");
	else if (token.type == AND) printf("AND");
	else if (token.type == WILDCARD) printf("WILDCARD");
	else if (token.type == CTRL_C) printf("CTRL_C");
	else if (token.type == CTRL_D) printf("CTRL_D");
	else if (token.type == CTRL_BACKSLASH) printf("CTRL_BACKSLASH");
	else if (token.type == INTEGER) printf("INTEGER: %lld", token.int_val);
	else if (token.type == LITERAL) printf("LITERAL: %s", token.str);
	else if (token.type == INTERPRETED) printf("INTERPRETED: %s", token.str);
	else if (token.type == REDIR) printf("REDIR: %lld", token.int_val);
	else if (token.type == SUBSHELL) printf("SUBSHELL: %s", token.str);
	else printf("Type not found");
}

bool	test_lexer_manualy(char *str)
{
	t_lexer			lexer;
	t_token			token;

	printf("test str: \"%s\"\n", str);
	lexer = new_lexer(str);
	token = next_token(&lexer);
	while (token.type != T_EOF)
	{
		printf("\t");
		print_token(token);
		printf("\n");
		token = next_token(&lexer);
	}
	printf("\t");
	print_token(token);
	printf("\n");
	return (true);
}

void test_token(t_lexer *lexer, t_type expected_type, const char *expected_str, int *passed, int *failed, int *test_nb)
{
    char *old_position = lexer->str + lexer->position;
    t_token token = next_token(lexer);
    (*test_nb)++;
    if (token.type == expected_type && (expected_str == NULL || ft_strcmp(token.str, expected_str) == 0)) {
        (*passed)++;
    } else {
        printf("Test %d failed(%s):\n", *test_nb, old_position);
		test_lexer_manualy(lexer->str);
        if (token.type != expected_type) {
            printf("\texpected: %s\n", type_to_string(expected_type));
            printf("\tactual: %s\n", type_to_string(token.type));
        } else {
            printf("\texpected: %s\n", expected_str);
            printf("\tactual: '%s'\n", token.str);
        }
        (*failed)++;
    }
}

void fixed_tests()
{
    int passed = 0, failed = 0, test_nb = 0;
    t_lexer lexer;

    // Test basic sign type
    lexer = new_lexer("");
    test_token(&lexer, T_EOF, NULL, &passed, &failed, &test_nb);

    // Test integer type
    lexer = new_lexer("123");
    test_token(&lexer, INTEGER, NULL, &passed, &failed, &test_nb);

    // Test literal type
    lexer = new_lexer("'abc'");
    test_token(&lexer, LITERAL, "abc", &passed, &failed, &test_nb);

    // Test interpreted type
    lexer = new_lexer("\"abc\"");
    test_token(&lexer, INTERPRETED, "abc", &passed, &failed, &test_nb);

    // Test built-in type
    lexer = new_lexer("echo");
    test_token(&lexer, FT_BUILDIN, "echo", &passed, &failed, &test_nb);

    // Test combinations
    lexer = new_lexer("echo 'abc'123");
    test_token(&lexer, FT_BUILDIN, "echo", &passed, &failed, &test_nb);
    test_token(&lexer, LITERAL, "abc", &passed, &failed, &test_nb);
    test_token(&lexer, INTEGER, NULL, &passed, &failed, &test_nb);

    lexer = new_lexer("echo 'abc' 123");
    test_token(&lexer, FT_BUILDIN, "echo", &passed, &failed, &test_nb);
    test_token(&lexer, LITERAL, "abc", &passed, &failed, &test_nb);
	test_token(&lexer, WHITE_SPACE, NULL, &passed, &failed, &test_nb);
    test_token(&lexer, INTEGER, NULL, &passed, &failed, &test_nb);


    // Test REDIR type with '<'
    lexer = new_lexer("<");
    test_token(&lexer, REDIR, NULL, &passed, &failed, &test_nb);

    // Test REDIR type with '<<'
    lexer = new_lexer("<<");
    test_token(&lexer, REDIR, NULL, &passed, &failed, &test_nb);

    // Test REDIR type with '>'
    lexer = new_lexer(">");
    test_token(&lexer, REDIR, NULL, &passed, &failed, &test_nb);

    // Test REDIR type with '>>'
    lexer = new_lexer(">>");
    test_token(&lexer, REDIR, NULL, &passed, &failed, &test_nb);

    printf("%d/%d tests passed\n", passed, passed + failed);
}

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		fixed_tests();
		return (0);
	}
	test_lexer_manualy(av[1]);
	return (0);
}


