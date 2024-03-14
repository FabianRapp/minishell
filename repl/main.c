/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:00:27 by frapp             #+#    #+#             */
/*   Updated: 2024/03/14 05:51:34 by mevangel         ###   ########.fr       */
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

void	free_child_data(t_child_data *data)
{
	my_free((void **)&(data->argv[0]));
	free(data->argv);
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
		free_child_data(&data);
		return ;
	}
	errno = 0;
	if (ast->pid != 0)
	{
		free_child_data(&data);
		return ;
	}
	
	// char **env_list = *(ast->envs);
	// int i = -1;
	// while(env_list[++i])
	// 	printf("%s$\n", env_list[i]);
	
	execve(data.path, data.argv, *(ast->envs));
	exit(errno);
}

void	add_global_data(t_ast *ast, t_env *env, char ***envs, char ***exp_list)
{
	if (!ast)
		return ;
	add_global_data(ast->left, env, envs, exp_list);
	add_global_data(ast->right, env, envs, exp_list);
	ast->env = env;
	ast->pipe[READ] = READ;
	ast->pipe[WRITE] = WRITE;
	ast->exit_status = DEFAULT_EXIT_STATUS;
	ast->envs = envs;
	ast->env_exp = exp_list;
}

bool	init_env(t_env *new_env, char **env_list)
{
	if (!new_env)
		return (false);
	if (!env_list)
		return (false);
	new_env->main_process = true;
	new_env->stop_execution = false;
	return (true);
}

int	main(int ac, char **av, char **base_env)
{
	t_ast			*ast;
	char			*input;
	t_cleanup_data	cleanup_data;
	t_env			env;
	char			**env_list;
	char			**exp_list;

	(void)av;
	errno = 0;
	// Initializes the standard in and out file descriptors by first cleaning
	// up and then duplicating the file descriptors for reading and writing:
	reset_stdio(RESET_STDIO_INIT);
	if (ac > 1)
		return (printf("No arguments allowed\n"), 0);
	// env.main_pid = get_pid();
	// if (!env.main_pid)
	// 	return (1);
	//? my stuff:
	env_list = ft_initialize_our_env(base_env);
	if (env_list == NULL)
		return (printf(PR_ERR "malloc failed\n"), 1);
	
	// int i = -1;
	// while(env_list[++i])
	// 	printf("%s$\n", env_list[i]);
	
	exp_list = ft_initialize_our_env(base_env);
	if (exp_list == NULL)
		return (ft_free_2darr(env_list), printf(PR_ERR "malloc failed\n"), 1);
	// //to test whethere i saved the env correctly:
	env.main_process = true;
	env.stop_execution = false;
	ast = get_input(&cleanup_data);
	//? shouldn't we protect the return of the get_input. in some cases you're returning NULL
	input = cleanup_data.input;
	while (1)
	{
		if (ast)
		{
			errno = 0;
			//print_ast(ast);
			add_global_data(ast, &env, &env_list, &exp_list);
			// printf("hello form ft_main\n");
			ast->cleanup_data = &cleanup_data;
			//print_ast(ast);
			run_node(ast);
			wait_all_children();
			main_exit(&cleanup_data, false, &env, -1);
		}
		if (LEAK_CHECK)
			system("leaks minishell"); //! we can not turn in that at the end. FORBIDDEN function
		ast = get_input(&cleanup_data);
		input = cleanup_data.input;
	}
	return (0);
}
