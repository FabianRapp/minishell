/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:00:27 by frapp             #+#    #+#             */
/*   Updated: 2024/01/29 10:19:17 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parser.h"

// if ast->info == NOT_FINISHED afterwards there was a command to execute
void	no_command(t_ast *ast)
{
	if (ast->name->token->type == DUMMY_COMMAND && (ast->redir_in || ast->redir_out))
	{
			//run redirs
			ast->exit_status = 0;
			ast->info = FINISHED;
	}
	else if (is_operator(ast->type))
	{
		if (ast->redir_in || ast->redir_out)
		{
			printf("DEBUG: operator with redirs in no_command()\n");
			ast->info = EXIT_ERROR;//DEBUG
			exit(1);// DEBUG
			return ;
		}
		ast->exit_status = 258;
		ast->info = SYNTAX_ERROR;
		// TODO: need to enable this condtion
		//if (not first command)
			//return (printf(SHELL_NAME": syntax error: unexpected end of file\n"), false);
		//else
		{
			print_error(1, "syntax error near unexpected token", "<needs to be converted to actual input:>", (char *)token_type_to_string(ast->type));
			return ;
		}
	}
	return ;
}

void	check_edgecases(t_ast *ast)
{
	no_command(ast);
	if (ast->info != NOT_FINISHED)
		return ;
	ft_buildin(ast);
	if (ast->info != NOT_FINISHED)
		return ;
}

void	create_child_command(t_ast *ast, char *command_path)
{
	pid_t	pid;
	char	**argv;

	pid = fork();
	if (pid == 0) // child process 
	{
		
		if (ast->info != NOT_FINISHED)
			exit(ast->exit_status);
		argv = ft_calloc(count_args(ast, ARGS) + 2, sizeof (char *const));
		if (!argv)
		{
			// cleanup()
			ast->info = EXIT_ERROR;
		}
		argv[0] = command_path;
		fill_args(ast, argv + 1, ARGS);
		execvp(command_path, argv);
		perror("command finished");
		ast->info = FINISHED;
		ast->exit_status = 0;
		exit(0);
	}
	else if (pid == -1) // error
	{
		ast->info = EXIT_ERROR;
		ast->exit_status = 126;
	}
	else // parent process
		waitpid(pid, &(ast->exit_status), 0);
}

// for now assumes ast to be the node of exactly one command
void	run_command_node(t_ast *ast, t_cleanup_data *cleanup_data)
{
	char	*path;
	char	*command_name;

	ast->cleanup_data = cleanup_data;
	expand_strs(ast);
	if (ast->info != NOT_FINISHED)
		return ;
	check_edgecases(ast);
	if (ast->info != NOT_FINISHED)
		return ;
	path = find_path(ast, &command_name, &(ast->info));
	if (ast->info == EXIT_ERROR)
		return ;
	else if (path == NULL)
	{
		print_error(SHELL_NAME, command_name, NULL, "command not found");
		ast->exit_status = 127;
		ast->info = SYNTAX_ERROR;
		return ;
	}
	create_child_command(ast, path);
	free(path);
}

int	main(void)
{
	t_ast			*ast;
	char			*input;
	t_cleanup_data	cleanup_data;

	ast = get_input(&cleanup_data);
	input = cleanup_data.input;
	while (input != NULL)
	{
		if (ast)
		{
			run_command_node(ast, &cleanup_data);
			//print_ast(ast);
			if (ast->info == EXIT_ERROR)
				return (main_cleanup(&cleanup_data), 1);
			else if (ast->info == SYNTAX_ERROR)
			{
			}
			print_ast(ast);
			//system("leaks minishell");
		}
		main_cleanup(&cleanup_data);
		ast = get_input(&cleanup_data);
		input = cleanup_data.input;
	}
	return (0);
}
