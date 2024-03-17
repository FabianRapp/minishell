/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:00:27 by frapp             #+#    #+#             */
/*   Updated: 2024/03/17 01:45:18 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parser.h"

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
	if (ft_buildin(ast) == true)
		return (true);
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
	if (!data->path || ast->exit_status != DEFAULT_EXIT_STATUS)
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
	if (ast->fd_to_close != INIT_VAL)
		close(ast->fd_to_close);
	if (ast->fd_to_close_write != INIT_VAL)
		close(ast->fd_to_close_write);
	if (ast->fd_to_close_read != INIT_VAL)
		close(ast->fd_to_close_read);
	//check_fds();
	if (execve(data.path, data.argv, *(ast->envs)) == -1)
		print_error("true", NULL, NULL, "execve failed\n");
	exit(errno);
}

void	add_global_data(t_ast *ast, t_env *env, char ***envs, t_cleanup_data *cleanup_data, char ***exp_list)
{
	if (!ast)
		return ;
	add_global_data(ast->left, env, envs, cleanup_data, exp_list);
	add_global_data(ast->right, env, envs, cleanup_data, exp_list);
	ast->fd_to_close = INIT_VAL;
	ast->fd_to_close_read = INIT_VAL;
	ast->fd_to_close_write = INIT_VAL;
	ast->env = env;
	ast->exit_status = DEFAULT_EXIT_STATUS;
	ast->envs = envs;
	ast->env_exp = exp_list;
	ast->cleanup_data = cleanup_data;
}

// bool	init_env(t_env *new_env, char **env_list)
// {
// 	if (!new_env)
// 		return (false);
// 	if (!env_list)
// 		return (false);
// 	new_env->main_process = true;
// 	new_env->stop_execution = false;
// 	return (true);
// }

t_result	init_main(int ac, t_env *env)
{
	errno = 0;
	set_last_exit(0);
	if (ac > 2)
	{
		print_error(true, NULL, NULL, "max one arg allowed");
		exit(1);
	}
	//env->main_pid = get_pid();
	if (!env->main_pid)
		return (1);
	if (!init_env(env))
		return (ERROR);
	return (SUCCESS);
}

void	check_exit_and_cleanup(t_cleanup_data *cleanup_data)
{
	if (full_exit_status(false) == true)
		main_exit(cleanup_data, true, false);
	else
		main_exit(cleanup_data, false, false);
}

int	main(int ac, char **av, char **base_env)
{
	t_ast			*ast;
	t_cleanup_data	cleanup_data;
	t_env			env;
	char			**env_list;
	char			**exp_list;

	if (init_main(ac, &env) == ERROR)
		return (1);
	(void)av;
	env_list = ft_initialize_our_env(base_env);
	if (env_list == NULL)
		return (get_last_exit());
	exp_list = ft_initialize_our_env(base_env);
	get_env(&env_list);
	if (exp_list == NULL)
		return (ft_free_2darr(env_list), get_last_exit());
	//if (ac >= 2)
		//ast = handle_manunal_input(av, &cleanup_data);
	//else
		ast = get_input(&cleanup_data);
	if (!ast)
		check_exit_and_cleanup(&cleanup_data);
	if (TESTER && !cleanup_data.input)
	{
		exit(get_last_exit());
	}
	while (1)
	{
		if (ast)
		{
			errno = 0;
			//print_ast(ast);
			add_global_data(ast, &env, &env_list, &cleanup_data, &exp_list);
			// printf("hello form ft_main\n");
			ast->cleanup_data = &cleanup_data;
			//print_ast(ast);
			run_node(ast);
			check_exit_and_cleanup(&cleanup_data);
			if (TESTER && !cleanup_data.input)
			{
				exit(get_last_exit());
			}
		}
		ast = get_input(&cleanup_data);
		if (!ast)
			check_exit_and_cleanup(&cleanup_data);
		if (TESTER && !ast && !cleanup_data.input)
		{
			exit(get_last_exit());
		}
	}
	return (0);
}
