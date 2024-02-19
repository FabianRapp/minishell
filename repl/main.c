/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:00:27 by frapp             #+#    #+#             */
/*   Updated: 2024/02/15 08:58:12 by frapp            ###   ########.fr       */
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
	if (!ast->name || ast->name->token->type == DUMMY_COMMAND)
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

void	clean_fds(t_fd_pair *pairs)
{
	int	i;

	i = 0;
	while (pairs + i && pairs[i].base_fd != INIT_VAL)
	{
		close(pairs[i].overload_with_fd);
		i++;
	}
}

// for now assumes ast to be the node of exactly one command
void	run_command_node(t_ast *ast)
{
	t_child_data	data;

	if (check_edgecases(ast))
		return ;
	init_child_data(&data, ast);
	if (ast->exit_status != DEFAULT_EXIT_STATUS)
		return ;
	
	ast->pid = fork();
	if (ast->pid == -1)
	{
		
		ast->exit_status = errno;
		print_error(true, NULL, NULL, strerror(ast->exit_status));
		errno = 0;
		return ;
	}
	errno = 0;
	if (ast->pid != 0)
	{
		// reset_fds();
		return ;
	}
	// if (resolve_redirs(ast) == ERROR)
	// 	return ;
	
	execve(data.path, data.argv, ast->envs);
	exit(errno);
}

void	add_global_data(t_ast *ast, t_env *env, char **envs)
{
	if (!ast)
		return ;
	add_global_data(ast->left, env, envs);
	add_global_data(ast->right, env, envs);
	ast->env = env;
	ast->pipe[READ] = READ;
	ast->pipe[WRITE] = WRITE;
	ast->exit_status = DEFAULT_EXIT_STATUS;
	ast->envs = envs;
}

int	main(int ac, char **av, char **base_env)
{
	t_ast			*ast;
	char			*input;
	t_cleanup_data	cleanup_data;
	t_env			env;

	int	base_fd[3];

	base_fd[WRITE] = dup(WRITE);
	base_fd[READ] = dup(READ);
	base_fd[STD_ERROR] = dup(STD_ERROR);
	errno = 0;
	if (ac > 1)
		return (printf("no args allowed\n"), 1);
	(void)av;
	//env.main_pid = get_pid();
	//if (!env.main_pid)
		//return (1);
	if (!init_env(&env, base_env))
		return (1);
	ast = get_input(&cleanup_data);
	input = cleanup_data.input;
	while (1)
	{
		if (ast)
		{
			errno = 0;
			//dup2(STD_ERROR, WRITE);
			//print_ast(ast);
			add_global_data(ast, &env, base_env);
			ast->cleanup_data = &cleanup_data;
			//print_ast(ast);
			run_node(ast);
			wait_all_children();
			//system("leaks minishell");
			main_exit(&cleanup_data, false, &env, true);
			// dup2(base_fd[WRITE], WRITE);
			// dup2(base_fd[READ], READ);
			//dup2(base_fd[STD_ERROR], STD_ERROR);
		}
		ast = get_input(&cleanup_data);
		input = cleanup_data.input;
	}
	return (0);
}
