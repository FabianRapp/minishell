/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:00:27 by frapp             #+#    #+#             */
/*   Updated: 2024/03/02 23:54:07 by mevangel         ###   ########.fr       */
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

char	**ft_initialize_our_env(char **base_env)
{
	int		i;
	char	**ret;

	i = 0;
	while (base_env[i])
		i++;
	ret = malloc((i + 1) * sizeof(char *));
	if (ret == NULL)
		return (NULL);
	ret[i] = NULL;
	i = -1;
	while (base_env[++i])
		ret[i] = ft_strdup(base_env[i]);
	return (ret);
}

int	main(int ac, char **av, char **base_env)
{
	t_ast			*ast;
	char			*input;
	t_cleanup_data	cleanup_data;
	t_env			env;
	char			**our_env;

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
	our_env = ft_initialize_our_env(base_env);
	if (our_env == NULL)
		return (printf(ERR "malloc failed\n"), 1);

	// //to test whethere i saved the env correctly:
	// int i = -1;
	// while(our_env[++i])
	// 	printf("%s$\n", our_env[i]);
	// // return (0);
	//? my stuff:end
	if (!init_env(&env, base_env))
		return (1);
	ast = get_input(&cleanup_data);
	input = cleanup_data.input;
	while (1)
	{
		if (ast)
		{
			errno = 0;
			//print_ast(ast);
			add_global_data(ast, &env, base_env);
			ast->cleanup_data = &cleanup_data;
			//print_ast(ast);
			run_node(ast);
			wait_all_children();
			main_exit(&cleanup_data, false, &env, -1);
		}
		if (LEAK_CHECK)
			system("leaks minishell");
		ast = get_input(&cleanup_data);
		input = cleanup_data.input;
	}
	return (0);
}
