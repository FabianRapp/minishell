/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugging.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 07:01:13 by frapp             #+#    #+#             */
/*   Updated: 2024/03/25 00:43:14 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../headers/lexer.h"
// #include "../headers/parser.h"

// DEBUG: unallowed function for debugging and finding leaks (fcntl)
// void	check_fds(void)
// {
// 	int		fd;
// 	int		open_fd_count = 0;

// 	int prev_errno = errno;
// 	fd = 3;
// 	while (fd < OPEN_MAX)
// 	{
// 		if (fcntl(fd, F_GETFD) != -1) 
// 		{
// 			printf("%d is open(fd): %s\n", fd, get_file_name(fd));
// 			open_fd_count++;
// 		}
// 		fd++;
// 	}
// 	errno = prev_errno;
// 	//if (LEAK_CHECK)// if some how dosnt work
// 	if (open_fd_count)
// 	{
// 		printf("open fds: %d\n", open_fd_count);
// 		print_fds();
// 	}
// }

// void print_colored(const char *text, int color_index)
// {
// 	char *colors[] = {RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE};
// 	int num_colors = sizeof(colors) / sizeof(colors[0]);
// 	char *color = colors[color_index % num_colors];
// 	printf("%s%s%s", color, text, RESET_COLOR);
// }

// char	*type_to_str(t_type type)
// {
// 	switch (type)
// 	{
// 		case UNKNOWN: return "UNKNOWN";
// 		case T_EOF: return "T_EOF";
// 		case WHITE_SPACE: return "WHITE_SPACE";
// 		case PIPE: return "PIPE";
// 		case OR: return "OR";
// 		case AND: return "AND";
// 		case ENV_VAR: return "ENV_VAR";
// 		case EXIT_STATUS_REQUEST: return "EXIT_STATUS_REQUEST";
// 		//case FT_BUILDIN: return "FT_BUILDIN";
// 		case WILDCARD: return "WILDCARD";
// 		//case CTRL_C: return "CTRL_C";
// 		//case CTRL_D: return "CTRL_D";
// 		//case CTRL_BACKSLASH: return "CTRL_BACKSLASH";
// 		case WORD: return "WORD";
// 		case INTERPRETED: return "INTERPRETED";
// 		case REDIR_IN: return "REDIR_IN";
// 		case REDIR_OUT: return "REDIR_OUT";
// 		case REDIR_APPEND: return "REDIR_APPEND";
// 		case HERE_DOC: return "HERE_DOC";
// 		case SUBSHELL: return "SUBSHELL";
// 		//case FLAG: return "FLAG";
// 		case COMMAND: return "COMMAND";
// 		case ARGUMENT: return "ARGUMENT";
// 		case REDIR_ARG: return "REDIR_ARG";
// 		case VOID: return "VOID";
// 		case DUMMY_COMMAND: return "DUMMY_COMMAND";
// 		case LITERAL: return "LITERAL";
// 		case PID_REQUEST: return "PID_REQUEST";
// 		case SEMICOL: return "SEMICOL";
// 		case HERE_STR: return "HERE_STR";
// 		default: return "Type not found";
// 	}
// }

// void	print_indent(int depth, bool left)
// {
// 	if (!left)
// 	{
// 		for (int i = 0; i < depth; i++) {
// 			print_colored(" ", depth);
// 		}
// 	}
// 	else
// 	{
// 		for (int i = 0; i < depth - 1; i++) {
// 			print_colored(" ", depth);
// 		}
// 		print_colored("│", depth - 1);
// 	}
// 	print_colored("│", depth);
// }

// void	print_indent_arg(int depth)
// {
// 	for (int i = 0; i < depth - 1; i++) {
// 		print_colored(" ", depth);
// 	}
// 	print_colored("├", depth - 1);
// 	print_colored("┐", depth);
// }

// //├ ─ ─ │  └──
// void	print_new_indent(int depth, bool left)
// {

// 	//print_indent(depth);
// 	//printf("\n");
// 	for (int i = 0; i < depth - 1; i++) {
// 		print_colored(" ", depth);
// 		//print_colored("─", depth);
// 	}
// 	if (depth > 0 && !left)
// 	{
// 		print_colored("└", depth - 1);
// 		print_colored("├─", depth);
// 	}
// 	else if (depth > 0 && left)
// 	{
// 		print_colored("├", depth - 1);
// 		print_colored("┬─", depth);
// 	}
// 	else
// 		print_colored("│", depth);
// }

// void print_parser_tree(t_parser *parser, const char *label, int depth)
// {
// 	if (parser) {
// 		while (parser && parser->p_type != T_EOF) {
// 			print_token(parser->token, parser, depth);
// 			parser = parser->next;
// 		}
// 		(void)label;
// 		// print_indent(depth);
// 		// print_colored("end ", depth);
// 		// print_colored(label, depth);
// 		// print_colored("\n", depth);
// 	}
// }

// void	print_token(t_token *token, t_parser *parser, int depth)
// {
// 	const char* token_type_str = type_to_str(token->type);

// 	printf("\n");
// 	print_indent(depth, false);
// 	if (!parser)
// 	{
// 		print_colored(token_type_str, depth);
// 		if (token->str_data)
// 			print_colored(token->str_data, depth);
// 		if (token->unknown)
// 			print_colored("unknown", depth);
// 		printf("\n");
// 	}
// 	else
// 	{
// 		const char* parser_type_str = type_to_str(parser->p_type);
// 		print_colored(parser_type_str, depth);
// 		print_colored("/", depth);
// 		print_colored(token_type_str, depth);
// 		print_colored(" ", depth);

// 		if (parser->rest_name) {
// 		//	print_parser_tree(parser->rest_name, "name", depth + 1);
// 		}
// 		if (parser->arg) {
// 			printf("\n");
// 			print_indent(depth, false);
// 			if (parser->arg->p_type == ARGUMENT)
// 				print_colored("Command arguments:", depth);
// 			else if (parser->arg->p_type == REDIR_ARG)
// 				print_colored("Redir argument:\n", depth);
// 			else
// 				print_colored("why is this here:\n", depth);
// 			print_parser_tree(parser->arg, "arg", depth);
// 		}
// 	}
// }

// bool	test_lexer_manualy(char *str)
// {
// 	t_lexer			lexer;
// 	t_token			*token;

// 	printf("test str: \"%s\"\n", str);
// 	printf("lexer output before in list:\n");
// 	lexer = new_lexer(str);
// 	token = next_new_token(&lexer, false);
// 	while (token->type != T_EOF)
// 	{
// 		printf("\t");
// 		print_token(token, NULL, 0);
// 		printf("\n");
// 		token = next_new_token(&lexer, false);
// 	}
// 	free(lexer.str);
// 	printf("\t");
// 	print_token(token, NULL, 0);
// 	return (true);
// }

// #include <sys/syslimits.h>
// #include <fcntl.h>

// char	*get_file_name(int fd)
// {
// 	char	*filePath = ft_calloc(1, PATH_MAX);
// 	char	*file_name;

// 	if (filePath == NULL)
// 		return NULL;
// 	int	old_errno = errno;
// 	if (fcntl(fd, F_GETPATH, filePath) != -1)
// 	{
// 		errno = old_errno;
// 		return (NULL);
// 	}
// 	file_name = extract_command_name(filePath);
// 	free(filePath);
// 	return (file_name);
// }

// void	print_fds(void)
// {
// 	t_fd_set	*fds;
// 	char		*base_fd_str;
// 	char		*over_load_fd_str;
// 	char		*backup_fd_str;

// 	printf("\n");
// 	fds = io_data(-1, NULL);
// 	printf("| base_fd | overload_with_fd | backup_fd |\n");
// 	//while (fds && fds->overload_with_fd != INIT_VAL)
// 	while(fds && !is_buffer_all_zeros(fds, sizeof(t_fd_set)))
// 	{
// 		base_fd_str = get_file_name(fds->base_fd);
// 		over_load_fd_str = get_file_name(fds->overload_with_fd);
// 		backup_fd_str = get_file_name(fds->base_fd_backup);
// 		printf("| %d: %s | %d: %s | %d: %s |\n", fds->base_fd,
// 			base_fd_str, fds->overload_with_fd,
// 				over_load_fd_str, fds->base_fd_backup,
// 				backup_fd_str);
// 		free(base_fd_str);
// 		free(over_load_fd_str);
// 		free(backup_fd_str);
// 		fds++;
// 	}
// 	printf("\n");
// }

// int	count_open_fds(void)
// {
// 	int			fd_count = 0;
// 	struct stat	stats;
// 	int			fd;
// 	int			err;

// 	fd = 0;
// 	err = errno;
// 	while (fd < FD_SETSIZE)
// 	{
// 		if (fstat(fd, &stats) == 0)
// 		{
// 			fd_count++;
// 		}
// 		fd++;
// 	}
// 	errno = err;
// 	return (FD_SETSIZE / 2 - fd_count);
// }

// void	print_parser(t_parser *parser, int tree_level)
// {
// 	t_parser *current = parser;

// 	while (current && current->p_type != T_EOF)
// 	{
// 		printf("next parser:\n");
// 		print_token(current->token, current, tree_level);
// 		printf("\n");
// 		if (parser->rest_name)
// 		{
// 			printf("---rest name:----\n");
// 			print_parser(parser->rest_name, tree_level);
// 		}
// 		if (parser->arg)
// 		{
// 			printf("---rest name:----\n");
// 			print_parser(parser->arg, tree_level);
// 		}
// 		current = current->next;
// 		tree_level += 2;
// 	}
// }

// void	print_token_list(t_token_list *token_node, int level)
// {
// 	while (token_node)
// 	{
// 		print_colored(type_to_str(token_node->token->type), level);
// 		print_colored(": ", level);
// 		print_colored(token_node->token->str_data, level);
// 		print_colored(" ; ", level);
// 		printf("\t");
// 		token_node = token_node->next;
// 	}
// }

// void	print_arg_list(t_arg *arg, int level, bool left)
// {
// 	print_indent_arg(level);
// 	print_colored(" Name:\t", level);
// 	while (arg)
// 	{
// 		printf("\n");
// 		print_indent(level, false);
// 		(void)left;
// 		print_token_list(arg->name, level);
// 		arg = arg->next;
// 	}
// }

// void	print_redir_list(t_redir *redir, int level, bool left)
// {
// 	t_arg	*arg;

// 	while (redir)
// 	{
// 		printf("\n");
// 		print_indent_arg(level);
// 		print_colored(type_to_str(redir->type), level);
// 		print_colored(": ", level);
// 		if (redir->type == HERE_DOC)
// 		{
// 			print_colored(redir->token_str_data, level);
// 		}
// 		else
// 		{
// 			arg = redir->arg;
// 			while (arg)
// 			{
// 				print_token_list(arg->name, level);
// 				arg = arg->next;
// 			}
// 		}
// 		redir = redir->next;
// 	}
// 	(void)left;
// }

// void	start_rec_print(t_ast *ast, int level, char *path, bool left)
// {
// 	if (!left)
// 		printf("\n");
// 	print_new_indent(level, left);
// 	print_colored("level: ", level);
// 	char	*a = ft_itoa(level / 2);
// 	print_colored(a, level);
// 	free(a);
// 	print_colored(" ; path: ", level);
// 	print_colored(path, level);
// 	printf("\n");
// 	print_indent(level, left);
// 	print_colored(type_to_str(ast->type), level);
// 	print_colored("; ", level);
// 	if (!(ast->name) && !(ast->redir) && !(ast->arg))
// 		printf("\n");
// 	if (ast->name)
// 	{
// 		//if (!is_redir(ast->type))
// 		{
// 			printf("\n");
// 			print_indent(level, left);
// 			print_colored("Name: ", level);
// 			print_token_list(ast->name, level);
// 		}
// 	}
// 	if (ast->redir)
// 	{
// 		printf("\n");
// 		print_indent(level, left);
// 		print_colored("Redir: ", level);
// 		print_redir_list(ast->redir, level + 1, left);
// 	}
// 	if (ast->arg)
// 	{
// 		printf("\n");
// 		print_indent(level, left);
// 		print_colored("Args: \n", level);
// 		print_arg_list(ast->arg, level + 1, left);
// 	}
// 	if (ast->left)
// 		start_rec_print(ast->left, level + 2, ft_strjoin(path, "->left"), true);
// 	if (ast->right)
// 		start_rec_print(ast->right, level + 2,
// 		ft_strjoin(path, "->right"), false);
// 	if (ft_strcmp(path, "root"))
// 		free (path);
// }

// void	print_ast(t_ast *ast)
// {
// 	start_rec_print(ast, 0, "root", false);
// 	printf("\n");
// }
