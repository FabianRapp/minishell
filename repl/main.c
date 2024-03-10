/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:00:27 by frapp             #+#    #+#             */
/*   Updated: 2024/03/10 14:40:26 by frapp            ###   ########.fr       */
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

// check errno when calling this
char	*extract_command_name(char *path)
{
	char	*name;
	int		i;
	char	**arr;

	if (ft_strncmp("/", path, 1) && ft_strncmp(".", path, 1) && ft_strncmp("..", path, 2))
		return (ft_strdup(path));
	arr = ft_split(path, '/');
	if (!arr)
		return (NULL);
	i = 0;
	while (arr[i])
	{
		i++;
	}
	if (!i)
		return (free_str_ar(arr), NULL);
	name = arr[i - 1];
	arr[i - 1] = NULL;
	free_str_ar(arr);
	return (name);
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
	//check_fds();
	execve(data.path, data.argv, ast->envs);
	print_error("true", NULL, NULL, "execve failed\n");
	exit(errno);
}

void	add_global_data(t_ast *ast, t_env *env, char **envs, t_cleanup_data *cleanup_data)
{
	if (!ast)
		return ;
	add_global_data(ast->left, env, envs, cleanup_data);
	add_global_data(ast->right, env, envs, cleanup_data);
	ast->fd_to_close = INIT_VAL;
	ast->env = env;
	ast->exit_status = DEFAULT_EXIT_STATUS;
	ast->envs = envs;
	ast->cleanup_data = cleanup_data;
}

t_result	init_main(int ac, char **base_env, t_env *env)
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
	if (!init_env(env, base_env))
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

	if (init_main(ac, base_env, &env) == ERROR)
		return (1);
	(void)av;
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
			//print_ast(ast);
			add_global_data(ast, &env, base_env, &cleanup_data);
			//print_ast(ast);
			run_node(ast);
			check_exit_and_cleanup(&cleanup_data);
			//if (TESTER)
				//exit(get_last_exit());
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
