/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 08:07:27 by frapp             #+#    #+#             */
/*   Updated: 2024/01/29 10:49:35 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	print_error(bool shell_name, char *command_name, char *arg, char *str)
{
	if (shell_name)
		ft_fprintf(2, "%s: ", SHELL_NAME);
	if (command_name)
		ft_fprintf(2, "%s: ", command_name);
	if (arg)
		ft_fprintf(2, "%s: ", arg);
	if (str)
		ft_fprintf(2, "%s", str);
	ft_fprintf(2, "\n");
	// if (shell_name)
	// 	ft_printf("%s: ", "minishell");//SHELL_NAME
	// //if (command_name)
	// //	ft_printf("%s: %s", command_name);
	// if (arg)
	// 	ft_printf("%s: ", arg);
	// if (str)
	// 	ft_printf("%s", str);
	// ft_printf("\n");
	(void)shell_name;
	(void)command_name;
	(void)arg;
	(void)str;
}

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
		printf("\t");
		token_node = token_node->next;
	}
}

void	print_arg_list(t_arg *arg, int level, bool left)
{
	//int	arg_nb;
	//int	base_lvl;

	//base_lvl = level;
	print_indent_arg(level);
	print_colored(" ; Name:\t", level);
	//arg_nb = 0;
	while (arg)
	{
		//level++;
		//print_indent_arg(level);
		printf("\n");
		print_indent(level, false);
		(void)left;
		//print_colored(token_type_to_string(arg->type), level);
		//print_colored(" : ", level);
		print_token_list(arg->name, level);
		arg = arg->next;
	}
}

void	start_rec_print(t_ast *ast, int level, char *path, bool left)
{
	if (!left)
		printf("\n");
	print_new_indent(level, left);
	print_colored("level: ", level);
	char	*a = ft_itoa(level / 2);
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
			print_token_list(ast->name, level);
		}
	}
	if (ast->redir_in)
	{
		printf("\n");
		print_indent(level, left);
		print_colored("Redir in: \n", level);
		print_arg_list(ast->redir_in, level + 1, left);
	}
	if (ast->redir_out)
	{
		printf("\n");
		print_indent(level, left);
		print_colored("Redir out: \n", level);
		print_arg_list(ast->redir_out, level + 1, left);
	}
	if (ast->arg)
	{
		printf("\n");
		print_indent(level, left);
		print_colored("Args: \n", level);
		print_arg_list(ast->arg, level + 1, left);
	}
	if (ast->left)
		start_rec_print(ast->left, level + 2, ft_strjoin(path, "->left"), true);
	if (ast->right)
		start_rec_print(ast->right, level + 2, ft_strjoin(path, "->right"), false);
	if (ft_strcmp(path, "root"))
		free (path);
}

void	print_ast(t_ast *ast)
{
	start_rec_print(ast, 0, "root", false);
	printf("\n");
}



void	cleanup()
{
	printf("clean up placeholder\n");
}

bool	is_termination_char(char c)
{
	if (c == 0 || c == '(' || c == ')' || c == '|' || c == '\'' || c == '\"' 
		|| c == '>' || c == '<' || c == '*' || c == '?' || c == '\'' 
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

int	name_len(char *str)
{
	int		len;

	if (!str)
		return (0);
	// /if (*str && *str != '_' && !ft_isalnum(*str))
	if (!*str || (*str != '_' && !ft_isalpha(*str)))
		return (0);
	len = 1;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
	{
		len++;
	}
	return (len);
}
