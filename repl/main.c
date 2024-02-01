/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:00:27 by frapp             #+#    #+#             */
/*   Updated: 2024/02/01 10:37:25 by frapp            ###   ########.fr       */
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
			print_error(1, "syntax error near unexpected token", "<needs to be converted to actual input:>", (char *)type_to_str_type(ast->type));
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

pid_t	create_child_command(t_ast *ast)
{
	pid_t	pid;
	char	**argv;
	char	*path;
	char	*command_name;

	pid = fork();
	if (pid != 0)
		return (pid);
	check_edgecases(ast);
	if (ast->info != NOT_FINISHED)
		exit(ast->exit_status);
	argv = ft_calloc(count_args(ast, ARGS) + 2, sizeof (char *const));
	if (!argv)
	{
		// cleanup()
		ast->info = EXIT_ERROR;
	}
	path = find_path(ast, &command_name, &(ast->info), "PATH");
	if (path == NULL && ast->info != EXIT_ERROR)
	{
		print_error(SHELL_NAME, command_name, NULL, "command not found");
		ast->exit_status = 127;
		ast->info = SYNTAX_ERROR;
	}
	else if (ast->info != EXIT_ERROR)
	{
		argv[0] = path;
		fill_args(ast, argv + 1, ARGS);
		execvp(path, argv);
		ast->info = FINISHED;
		ast->exit_status = 0;
	}
	exit(ast->exit_status);
	return (-1);
}

// for now assumes ast to be the node of exactly one command
void	run_command_node(t_ast *ast)
{
	pid_t	pid;
	int		temp;

	expand_strs(ast);
	if (ast->info != NOT_FINISHED)
		return ;
	if (!ft_strcmp(ast->name->token->str_data, "exit"))
	{
		ft_exit(ast);
		return ;
	}
	pid = create_child_command(ast);
	if (pid == -1)
	{
		ast->info = EXIT_ERROR;
		ast->exit_status = 126;
		*(ast->env->last_exit_status) = ast->exit_status;
		exit(ast->exit_status);
	}
	waitpid(pid, &(ast->exit_status), 0);
	ast->exit_status >>= 8; // idk why but this fixes the exit status. no idea wtf is happening to make this needed
	if (ast->exit_status == EXIT_ERROR)
	{
		printf("test\n");
		temp = ast->exit_status;
		main_cleanup(ast->cleanup_data, true, ast->env->main_process);
		exit(temp);
	}
	*(ast->env->last_exit_status) = ast->exit_status;
}

void disableRawMode(struct termios *orig_ter)
{
	struct termios *orig_termios;
	(void)orig_ter;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, orig_termios);
}

int	main(int ac, char **av, char **base_env)
{
	t_ast			*ast;
	char			*input;
	t_cleanup_data	cleanup_data;
	t_env			env;
	int				temp;
	//struct termios	terminal;

	env.last_exit_status = NULL;
	if (ac > 1)
		return (printf("no args allowed\n"), 1);
	(void)av;
	//atexit(() -> disableRawMode(&orig_termios));
	//tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminal);
	env.pid = get_pid();
	if (!env.pid)
		return (1);
	if (!init_env(&env, base_env))
		return (1);
	ast = get_input(&cleanup_data);
	input = cleanup_data.input;
	while (input != NULL)
	{
		if (ast)
		{
			env.last_exit_status = &(ast->exit_status);
			*(env.last_exit_status) = temp;
			ast->env = &env;
			print_ast(ast);
			walk_ast(ast, &cleanup_data);
			//print_ast(ast);
			//system("leaks minishell");
			temp = *(env.last_exit_status);
			main_cleanup(&cleanup_data, false, true);
		}
		ast = get_input(&cleanup_data);
		input = cleanup_data.input;
	}
	return (0);
}
