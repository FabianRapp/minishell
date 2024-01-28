/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:00:27 by frapp             #+#    #+#             */
/*   Updated: 2024/01/28 01:55:04 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parser.h"

bool	ft_buildin(char *command_name)
{
	(void)command_name;
	return (false);
}

bool	execute_command(char *command_path)
{
	pid_t	pid;

	pid = fork();
	if (pid) // parent process
	{
		
	}
	else // child process
	{
		execvp(command_path, {"Hello World!\n"});
	}
}


// for now assumes ast to be the node of exactly one command
bool	run_command_node(t_ast *ast)
{
	char	*path;
	char	*command_name;

	expand_strs(ast);
	command_name = ast->name->token->str_data; // TODO: name neeeds to be fully expanded, currtly some cases are not expanded (for example exit status req., wildcards etc)
	if (!command_name)
	{
		if (!ast->redir_in && ast->redir_out)
			return (printf("minishell: syntax error: unexpected end of file\n"), false);
		// do redirs
		return (true);
	}
	if (ft_buildin(command_name))
		return ;
	path = find_path(command_name);
	if (!path)
	{
		printf("minishell: %s: command not found\n", command_name);
		return (false);
	}
	free(path);
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
