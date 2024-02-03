/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:00:27 by frapp             #+#    #+#             */
/*   Updated: 2024/02/03 22:25:32 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parser.h"

void	child_cleanup(t_child_data *data)
{
	(void)data;
}


// if ast->info == NOT_FINISHED afterwards there was a command to execute
bool	no_command(t_ast *ast)
{
	if (ast->name->token->type == DUMMY_COMMAND && ast->redir)
	{
		if (!resolve_redirs(ast))
		{
			exit(1);
		}
		else
		{
			exit(0);
		}
	}
	else if (is_operator(ast->type))
	{
		if (ast->redir)
		{
			printf("DEBUG: operator with redirs in no_command()\n");
			exit(1);// DEBUG
			return (true);
		}
		// TODO: need to enable this condtion
		//if (not first command)
			//return (printf(SHELL_NAME": syntax error: unexpected end of file\n"), false);
		//else
		{
			print_error(1, "syntax error near unexpected token", "<needs to be converted to actual input:>", (char *)type_to_str_type(ast->type));
			exit(258);
		}
	}
	return (false);
}

bool	check_edgecases(t_ast *ast)
{
	if (no_command(ast))
		return (true);
	if (ft_buildin(ast))
		return (true);
	return (false);
}

void	init_child_data(t_child_data *data, t_ast *ast)
{
	
	data->path = NULL;
	data->command_name = NULL;
	data->exit_status = 0;
	data->malloc_error = false;
	data->argv = ft_calloc(count_args(ast->arg) + 2, sizeof (char *const));
	if (!data->argv)
	{
		data->exit_status = 1;
		return ;
	}
	data->path = find_path(ast, &(data->command_name), "PATH", data);
	if (data->exit_status)
		return ;
	data->argv[0] = data->path;
	fill_args(ast, data->argv + 1, ARGS);
}

pid_t	create_child_command(t_ast *ast)
{
	pid_t			pid;
	t_child_data	data;

	// if (!resolve_redirs(ast))
	// 	return (-1);
	init_child_data(&data, ast);
	if (data.exit_status)
	{
		set_status(ast, data.exit_status);
		return (-1);
	}
	if (data.path == NULL)
	{
		print_error(SHELL_NAME, data.command_name, NULL, "command not found");
		my_exit(ast, 127);
	}
	else
	{
		pid = fork();
		if (pid == -1)
		{
			print_error(true, NULL, NULL, "fork failed");
			my_exit(ast, errno);
		}
		if (pid != 0)
			my_exit(ast, 0);
		redir_stdio(ast);
		execvp(data.path, data.argv);
	}
	return (-1);
}

// for now assumes ast to be the node of exactly one command
void	run_command_node(t_ast *ast)
{
	ast->id = create_child_command(ast);
}

void disableRawMode(struct termios *orig_ter)
{
	struct termios *orig_termios;
	(void)orig_ter;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, orig_termios);
}

void	init_signals(struct sigaction *signals)
{
	signals->sa_handler = child_exit_handler;
	sigemptyset(&(signals->sa_mask));
	signals->sa_flags = SA_RESTART;
}

void	add_global_data(t_ast *ast, t_env *env)
{
	if (!ast)
		return ;
	add_global_data(ast->left, env);
	add_global_data(ast->right, env);
	ast->base_fd[IN] = dup(STDIN_FILENO);
	ast->base_fd[OUT] = dup(STDOUT_FILENO);
	if (ast->base_fd[IN] == -1 || ast->base_fd[OUT] == -1)
	{//handel error
	}
	ast->env = env;
	ast->fd[IN] = IN;
	ast->fd[OUT] = OUT;
}

int	main(int ac, char **av, char **base_env)
{
	t_ast			*ast;
	char			*input;
	t_cleanup_data	cleanup_data;
	t_env			env;
	//struct sigaction	signals;
	//struct termios	terminal;

	//init_signals(&signals);
	if (ac > 1)
		return (printf("no args allowed\n"), 1);
	(void)av;
	//atexit(() -> disableRawMode(&orig_termios));
	//tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminal);
	env.main_pid = get_pid();
	if (!env.main_pid)
		return (1);
	if (!init_env(&env, base_env))
		return (1);
	ast = get_input(&cleanup_data);
	input = cleanup_data.input;
	while (input != NULL)
	{
		if (ast)
		{
			add_global_data(ast, &env);
			ast->cleanup_data = &cleanup_data;
			print_ast(ast);
			run_node(ast);
			//system("leaks minishell");
			main_exit(&cleanup_data, false, &env, true);
		}
		ast = get_input(&cleanup_data);
		input = cleanup_data.input;
	}
	return (0);
}
