/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:42:58 by frapp             #+#    #+#             */
/*   Updated: 2024/01/17 10:41:28 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

bool	is_termination(char c)
{
	if (c == 0 || c == '(' || c == ')' || c == '|' || c == '\'' || c == '\"' 
		|| c == '>' || c == '<' || c == '*' || c == '?'
		|| c == '$' || c == '&'
		|| ft_iswhitespace(c))
	{
		return (true);
	}
	return (false);
}

int	name_len(char *str)
{
	int		len;

	if (*str != '_' && !ft_isalpha(*str))
		return (0);
	len = 0;
	while (ft_isalnum(str[len]) || str[len] == '_')
	{
		len++;
	}
	return (len);
}

bool	env_var_type(t_lexer *lexer, t_token *token)
{
	int	len;

	if (lexer->cur_char != '$')
		return (false);
	len = name_len((lexer->str) + lexer->position + 1);
	printf("len: %d\n", len);
	if (len == 0)
		return (false);
	token->type = ENV_VAR;
	token->str = ft_strndup((lexer->str) + lexer->position + 1 , len);
	lexer->read_position = lexer->position + 1 + len;
	return (token->type);
}


bool	basic_sign_type(t_lexer *lexer, t_token *token)
{
	if (lexer->cur_char == 0)
		token->type = T_EOF;
	else if (ft_iswhitespace(lexer->cur_char))
		token->type = WHITE_SPACE;
	else if (lexer->cur_char == '&' && (lexer->str)[lexer->position + 1] == '&')
	{
		token->type = AND;
		lexer->read_position = lexer->position + 2;
	}
	else if (lexer->cur_char == '|' && (lexer->str)[lexer->position + 1] == '|')
	{
		token->type = OR;
		lexer->read_position = lexer->position + 2;
	}
	else if (lexer->cur_char == '|')
		token->type = PIPE;
	else if (lexer->cur_char == '$' && (lexer->str)[lexer->position + 1] == '?')
	{
		token->type = EXIT_STATUS_REQUEST;
		lexer->read_position = lexer->position + 2;
	}
	else if (lexer->cur_char == '*')
		token->type = WILDCARD;
	else if (lexer->cur_char == 3) // might be wrong
		token->type = CTRL_C;
	else if (lexer->cur_char == 4) // might be wrong
		token->type = CTRL_D;
	else if (lexer->cur_char == 28) // might be wrong
		token->type = CTRL_BACKSLASH;
	return (token->type);
}

// // TODO:
// // handle >INT_MAX and < INT_MIN
// bool	integer_type(t_lexer *lexer, t_token *token)
// {
// 	if (lexer->cur_char == '0')
// 	{
// 		token->type = INTEGER;
// 		token->int_val = 0;
// 		return (token->type);
// 	}
// 	if (ft_atoi(lexer->str + lexer->position))
// 	{
// 		token->type = INTEGER;
// 		token->int_val = ft_atoi(lexer->str + lexer->position);
// 		while (ft_isdigit((lexer->str)[lexer->read_position]))
// 		{
// 			(lexer->read_position)++;
// 		}
// 		return (token->type);
// 	}
// 	return (token->type);
//}

bool	literal_type(t_lexer *lexer, t_token *token)
{
	size_t	len;

	if (lexer->cur_char != '\'')
		return (false);
	while ((lexer->str)[lexer->read_position] && (lexer->str)[lexer->read_position] != '\'')
	{
		(lexer->read_position)++;
	}
	if (!(lexer->str)[lexer->read_position])
		return (token->type);
	(lexer->read_position)++; // +1 to remove the signle quote
	len = lexer->read_position - lexer->position - 2; // -2 to remove the quotes
	token->type = LITERAL;
	token->str =  ft_strndup(lexer->str + lexer->position + 1, len); // +1 to skip the initial quote
	return (token->type);
}

bool	interpreted_type(t_lexer *lexer, t_token *token)
{
	size_t	len;

	if (lexer->cur_char != '\"')
		return (false);
	while ((lexer->str)[lexer->read_position] && (lexer->str)[lexer->read_position] != '\"')
	{
		(lexer->read_position)++;
	}
	if (!(lexer->str)[lexer->read_position])
		return (token->type);
	(lexer->read_position)++;
	len = lexer->read_position - lexer->position - 2; // -1 to remove the double quite
	token->type = INTERPRETED;
	token->str =  ft_strndup(lexer->str + lexer->position + 1, len); // +1 to remove the double quite
	return (token->type);
}

bool	ft_buildin_type(t_lexer *lexer, t_token *token)
{
	int	len;

	len = 0;
	if (!ft_strncmp(lexer->str + lexer->position, "echo -n", ft_strlen("echo -n")))
		len = ft_strlen("echo -n");
	else if (!ft_strncmp(lexer->str + lexer->position, "echo -", ft_strlen("echo -")))
		return (false);
	else if (!ft_strncmp(lexer->str + lexer->position, "echo", ft_strlen("echo")))
		len = ft_strlen("echo");
	else if (!ft_strncmp(lexer->str + lexer->position, "cd", ft_strlen("cd")))
		len = ft_strlen("cd");
	else if (!ft_strncmp(lexer->str + lexer->position, "pwd", ft_strlen("pwd")))
		len = ft_strlen("pwd");
	else if (!ft_strncmp(lexer->str + lexer->position, "export", ft_strlen("export")))
		len = ft_strlen("export");
	else if (!ft_strncmp(lexer->str + lexer->position, "unset", ft_strlen("unset")))
		len = ft_strlen("unset");
	else if (!ft_strncmp(lexer->str + lexer->position, "env", ft_strlen("env")))
		len = ft_strlen("env");
	else if (!ft_strncmp(lexer->str + lexer->position, "exit", ft_strlen("exit")))
		len = ft_strlen("exit");
	if (len)
	{
		token->type = FT_BUILDIN;
		token->str = ft_strndup(lexer->str + lexer->position, len);
		lexer->read_position = lexer->position + len;
	}
	return (token->type);
}

bool	redir_type(t_lexer *lexer, t_token *token)
{
	if (lexer->cur_char != '<' && lexer->cur_char != '>')
		return (false);
	token->type = REDIR;
	if (lexer->cur_char == '<')
	{
		if ((lexer->str)[lexer->position + 1] == '<')
		{
			token->int_val = 2;
			lexer->read_position += 1;
		}
		else
			token->int_val = 0;
	}
	else if (lexer->cur_char == '>')
	{
		if ((lexer->str)[lexer->position + 1] == '>')
		{
			token->int_val = 3;
			lexer->read_position += 1;
		}
		else
			token->int_val = 1;
	}
	return (token->type);
}

bool	subshell_type(t_lexer *lexer, t_token *token)
{
	int	count_open;

	if (lexer->cur_char != '(')
		return (false);
	count_open = 1;
	while((lexer->str)[lexer->read_position] && count_open)
	{
		if ((lexer->str)[lexer->read_position] == '(')
			count_open++;
		else if ((lexer->str)[lexer->read_position] == ')')
			count_open--;
		(lexer->read_position)++;
	}
	if (count_open)
	{
		lexer->read_position = lexer->position + 1;
		return (false);
	}
	token->type = SUBSHELL;
	token->str = ft_strndup(lexer->str + lexer->position + 1, lexer->read_position - lexer->position - 2);
	return (token->type);
}

bool	flag_type(t_lexer *lexer, t_token *token)
{
	if (lexer->cur_char != '-')
		return (false);
	if (is_termination(lexer->str[lexer->read_position]))
		return (0);
	token->type = FLAG;
	while (!is_termination(lexer->str[lexer->read_position]))
	{
		(lexer->read_position)++;
	}
	token->str = ft_strndup(lexer->str + lexer->position, lexer->read_position - lexer->position);
	return (token->type);
}

// has to run after all other typechecks
bool	word_type(t_lexer *lexer, t_token *token)
{
	if (is_termination(lexer->cur_char))
		return (0);
	while (!is_termination((lexer->str)[lexer->read_position]))
	{
		(lexer->read_position)++;
	}
	token->type = WORD;
	token->str = ft_strndup(lexer->str + lexer->position, lexer->read_position - lexer->position);
	return (token->type);
}
/*
	for cleanup:
	token.str has to be checked for NULL
	if not NULL "free(token.str)" has to be called
*/
#define DEBUG 0
t_token	next_token(t_lexer *lexer)
{
	t_token		token;

	zero_token(&token);
	if (basic_sign_type(lexer, &token))
	{
		if (DEBUG) printf("basic_sign_type\n");
	}
	else if (literal_type(lexer, &token))
	{
		if (DEBUG) printf("literal_type\n");
	}
	else if (interpreted_type(lexer, &token))
	{
		if (DEBUG) printf("interpreted_type\n");
	}
	else if (ft_buildin_type(lexer, &token))
	{
		if (DEBUG) printf("ft_buildin_type\n");
	}
	else if (redir_type(lexer, &token))
	{
		if (DEBUG) printf("redir_type\n");
	}
	else if (env_var_type(lexer, &token))
	{
		if (DEBUG) printf("env_var_type\n");
	}
	else if (subshell_type(lexer, &token))
	{
		if (DEBUG) printf("subshell_type\n");
	}
	// else if (integer_type(lexer, &token))
	// {
	// 	if (DEBUG) printf("integer_type\n");
	// }
	else if (flag_type(lexer, &token))
	{
		if (DEBUG) printf("flag_type\n");
	}
	else if (word_type(lexer, &token))
	{
		if (DEBUG) printf("word_type\n");
	}
	else if (DEBUG)
	{
		printf("no function IDed the type\n");
	}

	if (token.type == UNKNOWN)
		token.unknown = lexer->cur_char;
	read_char(lexer);
	return (token);
}

// inits a lexer object, returns the object
// NOT a pointer to a dynamic memory location!!
// takes a NULL-terminated str
t_lexer	new_lexer(char *str)
{
	t_lexer		lexer;

	lexer.position = 0;
	lexer.read_position = 0;
	lexer.str = str;
	read_char(&lexer);
	return (lexer);
}
