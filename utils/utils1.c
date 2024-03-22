/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 08:07:27 by frapp             #+#    #+#             */
/*   Updated: 2024/03/22 01:55:10 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	ft_close(int *fd)
{
	int	ret;

	ret = 0;
	if (*fd != INIT_VAL)
		ret = close(*fd);
	*fd = INIT_VAL;
	return (ret);
}

int	count_open_fds(void)
{
	int			fd_count = 0;
	struct stat	stats;
	int			fd;
	int			err;

	fd = 0;
	err = errno;
	while (fd < FD_SETSIZE)
	{
		if (fstat(fd, &stats) == 0)
		{
			fd_count++;
		}
		fd++;
	}
	errno = err;
	return (FD_SETSIZE / 2 - fd_count);
}

void	add_token_back_node(t_token_list **list, t_token_list *new_node)
{
	if (!*list)
	{
		*list = new_node;
		return ;
	}
	while ((*list)->next)
	{
		list = &((*list)->next);
	}
	(*list)->next = new_node;
	return ;
}

t_result	add_token_back(t_token_list **list, t_token *token)
{
	t_token_list	*new_node;

	new_node = ft_calloc(1, sizeof(t_token_list));
	if (!new_node)
		return (ERROR);
	new_node->token = token;
	if (!*list)
	{
		*list = new_node;
		return (SUCCESS);
	}
	while ((*list)->next)
	{
		list = &((*list)->next);
	}
	(*list)->next = new_node;
	return (SUCCESS);
}

void	free_token_list(t_token_list *list)
{
	t_token_list	*last;

	while (list)
	{
		last = list;
		list = list->next;
		free_token(last->token);
		free(last);
	}
}

void	free_arg_list(t_arg *list)
{
	t_arg	*last;

	while (list)
	{
		last = list;
		list = list->next;
		free_token_list(last->name);
		free(last);
	}
}

t_result	insert_whitespace_end(t_token_list **list)
{
	t_token	*new;

	new = ft_calloc(1, sizeof(t_token));
	if (!new)
		return (ERROR);
	new->type = WHITE_SPACE;
	if (add_token_back(list, new) == ERROR)
		return (free(new), ERROR);
	return (SUCCESS);
}

t_result	insert_whitespace_before(t_token_list **head)
{
	t_token_list	*new;

	new = ft_calloc(1, sizeof(t_token_list));
	if (!new)
		return (ERROR);
	new->token = ft_calloc(1, sizeof(t_token));
	if (!new->token)
		return (free(new), ERROR);
	new->token->type = WHITE_SPACE;
	add_token_node_front(head, new);
	return (SUCCESS);
}

void	add_token_node_front(t_token_list **head, t_token_list *new)
{
	t_token_list	*temp;

	temp = *head;
	*head = new;
	new->next = temp;
}

t_result	errno_to_result(void)
{
	if (errno)
		return (ERROR);
	return (SUCCESS);
}

t_result	set_errno_as_exit(t_ast *ast, bool msg)
{
	ast->exit_status = errno;
	set_last_exit(errno);
	if (errno)
	{
		if (msg)
			print_error(true, NULL, NULL, strerror(errno));
		errno = 0;
		return (ERROR);
	}
	return (SUCCESS);
}

int	line_counter(void)
{
	static int	line_count = -1;

	line_count++;
	return (line_count);
}

char	*ft_read_line(char *header)
{
	char	*line;
	char	*temp;

	if (!isatty(0))
	{
		temp = get_next_line(0);
		line = ft_strtrim(temp, "\n");
		free(temp);
	}
	else
		line = readline(header);
	line_counter();
	return (line);
}

// TODO make set_last_exit only run if the children did not exit yet when this function was called
t_result	wait_all_children(t_ast *ast)
{
	int		status;

	errno = 0;
	if (ast && ast->pid != INIT_VAL && ast->exit_status == INIT_VAL)
		waitpid(ast->pid, &status, 0);
	// if (ast->type == PIPE && ast->left->exit_status == DEFAULT_EXIT_STATUS && ast->left->pid != INIT_VAL)
	// 	waitpid(ast->left->pid, &(ast->left->exit_status), WNOHANG);
	// if (ast->type == PIPE && ast->left->exit_status == DEFAULT_EXIT_STATUS && ast->left->pid != INIT_VAL)
	// 	kill(ast->left->pid, SIGINT);
	while (errno != ECHILD)
	{
		waitpid(-1, &status, 0);
		if (errno != ECHILD)
		{
			//set_last_exit(WEXITSTATUS(status));
		}
	}
	errno = 0;
	return (SUCCESS);
}

t_token	*new_dummy_token(void)
{
	t_token	*dummy;

	dummy = ft_calloc(1, sizeof(t_token));
	if (!dummy)
		return (NULL);
	dummy->type = DUMMY_COMMAND;
	return (dummy);
}

void	print_error(bool shell_name, char *command_name, char *arg, char *str)
{
	//if (!TESTER)
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
	}
}

//bash prints: bash: export: `4add=hi': not a valid identifier
// and with the above function i was missing the quotes
void	print_error_addsq(bool shell_name, char *command_name, char *arg, char *str)
{
	if (shell_name)
		ft_fprintf(2, "%s: ", SHELL_NAME);
	if (command_name)
		ft_fprintf(2, "%s: ", command_name);
	if (arg)
		ft_fprintf(2, "`%s': ", arg);
	if (str)
		ft_fprintf(2, "%s", str);
	ft_fprintf(2, "\n");
}

bool	ft_free(void **ptr)
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

	while (current && current->p_type != T_EOF)
	{
		printf("next parser:\n");
		print_token(current->token, current, tree_level);
		printf("\n");
		if (parser->rest_name)
		{
			printf("---rest name:----\n");
			print_parser(parser->rest_name, tree_level);
		}
		if (parser->arg)
		{
			printf("---rest name:----\n");
			print_parser(parser->arg, tree_level);
		}
		current = current->next;
		tree_level += 2;
	}
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
		print_colored(type_to_str_type(token_node->token->type), level);
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
	print_colored(" Name:\t", level);
	//arg_nb = 0;
	while (arg)
	{
		//level++;
		//print_indent_arg(level);
		printf("\n");
		print_indent(level, false);
		(void)left;
		//print_colored(type_to_str_type(arg->type), level);
		//print_colored(" : ", level);
		print_token_list(arg->name, level);
		arg = arg->next;
	}
}

void	print_redir_list(t_redir *redir, int level, bool left)
{
	t_arg	*arg;

	while (redir)
	{
		printf("\n");
		print_indent_arg(level);
		print_colored(type_to_str_type(redir->type), level);
		print_colored(": ", level);
		if (redir->type == HERE_DOC)
		{
			print_colored(redir->token_str_data, level);
		}
		else
		{
			arg = redir->arg;
			while (arg)
			{
				print_token_list(arg->name, level);
				arg = arg->next;
			}
		}
		redir = redir->next;
	}
	(void)left;
}

bool	is_operator(t_type type)
{
	if (type == PIPE || type == OR || type == AND || type == SEMICOL)
	{
		return (true);
	}
	return (false);
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
	print_colored(type_to_str_type(ast->type), level);
	print_colored("; ", level);
	if (!(ast->name) && !(ast->redir) && !(ast->arg))
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
	if (ast->redir)
	{
		printf("\n");
		print_indent(level, left);
		print_colored("Redir: ", level);
		print_redir_list(ast->redir, level + 1, left);
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

void	free_token(t_token *token)
{
	if (!token)
		return ;
	ft_free((void **)&(token->old_data));
	ft_free((void **)&(token->str_data));
	ft_free((void **)&(token->left_redir_arg));
	free(token);
}

// check errno when calling this
char	*extract_command_name(char *path)
{
	char	*name;
	int		i;
	char	**arr;

	if (ft_strncmp("/", path, 1) && ft_strncmp(".", path, 1) && ft_strncmp("..", path, 2))
		return (ft_strdup(path));
	arr = ft_split(path, '/');
	if (!arr)
		return (NULL);
	i = 0;
	while (arr[i])
	{
		i++;
	}
	if (!i)
		return (ft_free_2darr(arr), NULL);
	name = arr[i - 1];
	arr[i - 1] = NULL;
	ft_free_2darr(arr);
	return (name);
}

bool	is_termination_char(char c)
{
	if (c == 0 || c == '(' || c == ')' || c == '|' || c == '\'' || c == '\"' 
		|| c == '>' || c == '<' 
		|| c == '$' || c == '&'
		|| ft_iswhitespace(c)
		|| c == ';')
	{
		return (true);
	}
	return (false);
}

bool	is_wildcard_block_termination(char c)
{
	if (c == 0 || c == '(' || c == ')' || c == '|'
		|| c == '>' || c == '<'
		|| c == '$' || c == '&'
		|| ft_iswhitespace(c)
		|| c == ';')
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
