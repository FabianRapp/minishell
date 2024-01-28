/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:00:27 by frapp             #+#    #+#             */
/*   Updated: 2024/01/28 04:20:51 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parser.h"

int	count_args(t_ast *ast, int type)
{
	t_arg	*args;
	int		count;

	if (type == ARGS)
		args = ast->arg;
	else if (type == OUT)
		args = ast->redir_out;
	else if (type == IN)
		args = ast->redir_in;
	else
		return (0);
	count = 0;
	while (args && args->type != T_EOF)
	{
		count++;
		args = args->next;
	}
	return (count);
}

void	fill_args(t_ast *ast, char *argv[], int type)
{
	t_arg	*args;
	int		i;

	if (type == ARGS)
		args = ast->arg;
	else if (type == REDIR_OUT)
		args = ast->redir_out;
	else if (type == REDIR_IN)
		args = ast->redir_in;
	else
		return ;
	i = 0;
	while (args && args->type != T_EOF)
	{
		argv[i++] = args->name->token->str_data; // assumes name to be fully expanded (not the case right now)
		args = args->next;
	}
	argv[i] = NULL;
}

int	execute_command(char *command_path, t_ast *ast)
{
	pid_t	pid;
	int		status;
	char	**argv;

	pid = fork();
	status = 0;
	if (pid == 0) // child process 
	{
		argv = ft_calloc(count_args(ast, ARGS) + 2, sizeof (char *const));
		argv[0] = command_path;
		if (!argv)
		{
			perror("malloc fail");
			exit(1);
		}
		fill_args(ast, argv + 1, ARGS);
		execvp(command_path, argv);
		perror("command finished");
		exit(0);
	}
	else if (pid == -1) // error
	{

	}
	else // parent process
	{
		waitpid(pid, &status, 0);
	}
	return (status);
}

// for now assumes ast to be the node of exactly one command
bool	run_command_node(t_ast *ast)
{
	//char	*path;
	//char	*command_name;

	expand_strs(ast);
	if (ft_buildin(ast))
		return (true);
	// command_name = ast->name->token->str_data; // TODO: name neeeds to be fully expanded, currtly some cases are not expanded (for example exit status req., wildcards etc)
	// if (!command_name)
	// {
	// 	if (!ast->redir_in || !ast->redir_out)
	// 		return (printf("minishell: syntax error: unexpected end of file\n"), false);
	// 	// do redirs
	// 	return (true);
	// }
	// path = find_path(command_name);
	// if (!path)
	// {
	// 	printf("minishell: %s: command not found\n", command_name);
	// 	return (false);
	// }
	// ast->return_val = execute_command(path, ast);
	// ast->finished = true;
	// free(path);
	return (true);
}

int	main(void)
{
	t_ast	*ast;
	char	*input;

	input = readline("minishell: ");
	while (input != NULL)
	{
		if (*input)
		{
			add_history(input);
			ast = parser(input);
			if (ast)
			{
				//print_ast(ast);
				run_command_node(ast);
				print_ast(ast);
				free_ast(ast);
			}
			//system("leaks minishell");
		}
		free(input);
		input = readline("minishell: ");
	}
	return (0);
}
