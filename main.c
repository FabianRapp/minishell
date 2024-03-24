/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:00:27 by frapp             #+#    #+#             */
/*   Updated: 2024/03/24 02:08:40 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minishell.h"
#include "headers/parser.h"

bool	no_command(t_ast *ast)
{
	if (!ast->name || ast->name->token->type == DUMMY_COMMAND)
	{
		ast->exit_status = 0;
		return (true);
	}
	return (false);
}

bool	check_edgecases(t_ast *ast)
{
	if (no_command(ast) == true)
		return (true);
	if (!ast->dont_run_buildins && ft_builtin_control(ast) == true)
	{
		return (true);
	}
	return (false);
}

void	init_child_data(t_child_data *data, t_ast *ast)
{
	data->argv = NULL;
	data->path = NULL;
	data->command_name = NULL;
	data->argv = ft_calloc(count_args(ast->arg) + 2, sizeof (char *const));
	if (!data->argv)
	{
		ast->exit_status = errno;
		return ;
	}
	data->command_name = ast->name->token->str_data;
	data->path = find_path(ast, data->command_name, "PATH");
	data->command_name = NULL;
	if (!data->path)
	{
		ast->exit_status = get_last_exit();
		return ;
	}
	if (ast->exit_status != DEFAULT_EXIT_STATUS)
		return ;
	data->argv[0] = extract_command_name(data->path); // TODO: mall error
	data->command_name = extract_command_name(data->path); // TODO: mall error
	fill_args(ast, data->argv + 1, ARGS);
}

void	free_child_data(t_child_data *data)
{
	ft_free((void **)&(data->argv[0]));
	free(data->command_name);
	free(data->argv[0]);
	free(data->argv);
	free(data->path);
}

void	run_command_node(t_ast *ast)
{
	t_child_data	data;

	if (check_edgecases(ast))
		return ;
	init_child_data(&data, ast);
	if (!data.path || ast->exit_status != DEFAULT_EXIT_STATUS)
	{
		free_child_data(&data);
		return ;
	}
	ast->pid = fork();
	errno = 0;
	if (ast->pid == -1)
	{
		ast->exit_status = errno;
		print_error(true, NULL, NULL, strerror(ast->exit_status));
		errno = 0;
		free_child_data(&data);
		return ;
	}
	if (ast->pid != 0)
	{
		free_child_data(&data);
		return ;
	}
	ft_close(&(ast->fd_to_close));
	ft_close(&(ast->fd_to_close_write));
	ft_close(&(ast->fd_to_close_read));
	//check_fds();
	if (ast->shared_data->envs)
	{
		if (execve(data.path, data.argv, *(ast->shared_data->envs)) == -1)
			print_error("true", data.command_name, NULL, strerror(errno));
	}
	else
	{
		if (execve(data.path, data.argv, NULL) == -1)
			print_error("true", data.command_name, NULL, strerror(errno));
	}
	exit(errno);
}

void	add_global_data(t_ast *ast, t_shared_data *shared_data)
{
	if (!ast)
		return ;
	add_global_data(ast->left, shared_data);
	add_global_data(ast->right, shared_data);
	ast->fd_to_close = INIT_VAL;
	ast->fd_to_close_read = INIT_VAL;
	ast->fd_to_close_write = INIT_VAL;
	ast->shared_data = shared_data;
	ast->exit_status = DEFAULT_EXIT_STATUS;
}

static bool	init_shared_data(t_shared_data *new_env)
{
	if (!new_env)
		return (false);
	new_env->stop_execution = false;
	return (true);
}

t_result	init_main(int ac, t_shared_data *shared_data)
{
	errno = 0;
	set_last_exit(0);
	shared_data->env_exp = NULL;
	shared_data->envs = NULL;
	set_ctrl_slash(&(shared_data->sig_set));
	set_ctrl_c(&(shared_data->sig_set));
	if (ac > 2)
	{
		print_error(true, NULL, NULL, "max one arg allowed");
		exit(1);
	}
	//shared_data->main_pid = get_pid();
	if (!shared_data->main_pid)
		return (1);
	if (!init_shared_data(shared_data))
		return (ERROR);
	return (SUCCESS);
}

int	main(int ac, char **av, char **base_env)
{
	t_ast			*ast;
	t_cleanup_data	cleanup_data;
	t_shared_data	shared_data;
	char			**env_list;
	char			**exp_list;

	cleanup_data.shared_data = &shared_data;
	if (init_main(ac, &shared_data) == ERROR)
		return (1);
	(void)av;
	env_list = ft_initialize_env(base_env);
	if (env_list == NULL)
		return (get_last_exit());
	exp_list = ft_initialize_env(base_env);
	if (exp_list == NULL)
		return (ft_free_2darr(env_list), get_last_exit());
	get_env_list(&env_list);
	shared_data.envs = &env_list;
	shared_data.env_exp = &exp_list;
	shared_data.cleanup_data = &cleanup_data;
	ast = get_input(&cleanup_data);
	if (!ast)
		main_exit(&cleanup_data, full_exit_status(false) == true, false);
	if (TESTER && !cleanup_data.input)
		main_exit(&cleanup_data, true, false);
	while (1)
	{
		if (ast)
		{
			errno = 0;
			//print_ast(ast);
			add_global_data(ast, &shared_data);

			// printf("hello form ft_main\n");
			ast->shared_data->cleanup_data = &cleanup_data;
			//print_ast(ast);
			run_node(ast);
			main_exit(&cleanup_data, full_exit_status(false) == true, false);
		}
		ast = get_input(&cleanup_data);
		if (!ast)
			main_exit(&cleanup_data, full_exit_status(false) == true, false);
	// system("leaks minishell");
	// exit(1);
	}
	return (0);
}
