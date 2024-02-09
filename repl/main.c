/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:00:27 by frapp             #+#    #+#             */
/*   Updated: 2024/02/09 21:06:06 by frapp            ###   ########.fr       */
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
	if (ast->name->token->type == DUMMY_COMMAND)
	{
		if (!ast->redir)
			ast->exit_status = 1;
		ast->exit_status = 0;
		set_last_exit(ast->exit_status);
		return (true);
	}
	else if (is_operator(ast->type))
	{
		if (ast->redir)
		{
			printf("DEBUG: operator with redirs in no_command()\n");
			ast->exit_status = 1;
			set_last_exit(ast->exit_status);
			return (true);
		}
		// TODO: need to enable this condtion
		//if (not first command)
			//return (printf(SHELL_NAME": syntax error: unexpected end of file\n"), false);
		//else
		{
			print_error(1, "syntax error near unexpected token", "<needs to be converted to actual input:>", (char *)type_to_str_type(ast->type));
			ast->exit_status = 258;
			set_last_exit(ast->exit_status);
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
	data->command_name = ast->name->token->str_data;
	data->malloc_error = false;
	data->argv = ft_calloc(count_args(ast->arg) + 2, sizeof (char *const));
	if (!data->argv)
	{
		ast->exit_status = errno;
		return ;
	}
	data->path = find_path(ast, data->command_name, "PATH");
	if (ast->exit_status != DEFAULT_EXIT_STATUS)
		return ;
	data->argv[0] = data->path;
	fill_args(ast, data->argv + 1, ARGS);
}

// for now assumes ast to be the node of exactly one command
void	run_command_node(t_ast *ast)
{
	t_child_data	data;

	init_child_data(&data, ast);
	if (ast->exit_status != DEFAULT_EXIT_STATUS)
		return ;
	ast->pid = fork();
	if (ast->pid == -1)
	{
		ast->exit_status = errno;
		print_error(true, NULL, NULL, strerror(ast->exit_status));
		return ;
	}
	if (ast->pid != 0)
	{
		//waitpid(ast->pid, &(ast->exit_status), 0);
		//ast->exit_status >>= 8;
		//exit( ast->exit_status);
		return ;
	}
	redir_stdio(ast);
	execve(data.path, data.argv, ast->envs);
	exit(errno);
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

void	add_global_data(t_ast *ast, t_env *env, char **envs)
{
	if (!ast)
		return ;
	add_global_data(ast->left, env, envs);
	add_global_data(ast->right, env, envs);
	//ast->base_fd[IN] = dup(STDIN_FILENO);
	//ast->base_fd[OUT] = dup(STDOUT_FILENO);
	//if (ast->base_fd[IN] == -1 || ast->base_fd[OUT] == -1)
	{//handel error
	}
	ast->env = env;
	ast->fd[IN] = IN;
	ast->fd[OUT] = OUT;
	// ast->fd[IN] = ast->base_fd[IN];
	// ast->fd[OUT] = ast->base_fd[OUT];
	ast->exit_status = DEFAULT_EXIT_STATUS;
	ast->envs = envs;
}

int	main(int ac, char **av, char **base_env)
{
	t_ast			*ast;
	char			*input;
	t_cleanup_data	cleanup_data;
	t_env			env;
	int				status;

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
	while (1)
	{
		if (ast)
		{
			//print_ast(ast);
			add_global_data(ast, &env, base_env);
			ast->cleanup_data = &cleanup_data;
			//print_ast(ast);
			run_node(ast);
			while (waitpid(-1, &status, 0) != -1)
			{
				set_last_exit(WEXITSTATUS(status));
			}
			//system("leaks minishell");
			main_exit(&cleanup_data, false, &env, true);
		}
		ast = get_input(&cleanup_data);
		input = cleanup_data.input;
	}
	return (0);
}
