/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 08:07:27 by frapp             #+#    #+#             */
/*   Updated: 2024/01/27 00:27:02 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

bool	my_free(void **ptr)
{
	if (!ptr)
		return (false);
	if (!*ptr)
		return (false);
	free(*ptr);
	*ptr = NULL;
	return (true);
}

void	print_parser(t_parser *parser, int tree_level)
{
	t_parser *current = parser;
	do
	{
		if (current->p_type == T_EOF)
			break ;
		print_token(current->token, current, tree_level);
		current = current->next;
	} while (current != parser && current);
	//if (!current)
		//printf("parser object 1 or 0 nodes (should be 1)\n");
	// if (current->p_type == T_EOF)
	// {
	// 	print_token(current->token, current, tree_level);
	// 	printf("\n");
	// }
	//free_parser_main(parser);
}

void	print_token_list(t_token_list *token_node, int level)
{
	while (token_node)
	{
		print_colored(token_type_to_string(token_node->token->type), level);
		print_colored(": ", level);
		print_colored(token_node->token->str_data, level);
		print_colored(" ; ", level);
		printf("\n");
		token_node = token_node->next;
	}
}

void	print_arg_list(t_arg *arg, int level, bool left)
{
	//int	arg_nb;

	//arg_nb = 0;
	while (arg)
	{
		level++;
		print_indent_arg(level);
		(void)left;
		print_colored(token_type_to_string(arg->type), level);
		print_colored(" ; Name : ", level);
		print_token_list(arg->name, level);
		arg = arg->next;
	}
}

void	start_rec_print(t_ast *ast, int level, char *path, bool left)
{
	t_token_list	*token_node;

	print_new_indent(level, left);
	print_colored("level: ", level);
	char	*a = ft_itoa(level);
	print_colored(a, level);
	free(a);
	print_colored(" ; path: ", level);
	print_colored(path, level);
	printf("\n");
	print_indent(level, left);
	print_colored(token_type_to_string(ast->type), level);
	print_colored("; ", level);
	if (!(ast->name) && !(ast->redir_in) && !(ast->redir_out) && !(ast->arg))
		printf("\n");
	if (ast->name)
	{
		//if (!is_redir(ast->type))
		{
			printf("\n");
			print_indent(level, left);
			print_colored("Name: ", level);
			token_node = ast->name;
			print_token_list(token_node, level);
		}
	}
	if (ast->redir_in)
	{
		print_indent(level, left);
		print_colored("Redir in: \n", level);
		print_arg_list(ast->redir_in, level, left);
	}
	if (ast->redir_out)
	{
		print_indent(level, left);
		print_colored("Redout out: \n", level);
		print_arg_list(ast->redir_out, level, left);
	}
	if (ast->arg)
	{
		print_indent(level, left);
		print_colored("Args: \n", level);
		print_arg_list(ast->arg, level, left);
	}
	if (ast->left)
		start_rec_print(ast->left, level + 1, ft_strjoin(path, "->left"), true);
	if (ast->right)
		start_rec_print(ast->right, level + 1, ft_strjoin(path, "->right"), false);
	if (ft_strcmp(path, "root"))
		free (path);
}

void	print_ast(t_ast *ast)
{
	start_rec_print(ast, 0, "root", false);
}



void	cleanup()
{
	printf("clean up placeholder\n");
}

bool	is_termination_char(char c)
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


// mb usefull later, for parser not useable
bool	valid_path_syntax(t_token *token)
{
	int	len;

	if (!token->str_data)
		return (0);
	len = ft_strlen(token->str_data);
	if (!token->str_data || *(token->str_data) == 0 || len > PATH_MAX)
		return (false);
	// check max individual file name length:
	while (len && (token->str_data)[len - 1] != '/')
		len--;
	if (ft_strlen(token->str_data) - len > NAME_MAX)
		return (false);
	return (true);
}

