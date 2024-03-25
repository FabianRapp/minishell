/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:00:27 by frapp             #+#    #+#             */
/*   Updated: 2024/03/25 06:13:58 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minishell.h"
#include "headers/parser.h"

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
	set_signals();
	set_last_exit(0);
	shared_data->env_exp = NULL;
	shared_data->envs = NULL;
	if (ac > 2)
	{
		print_error(true, NULL, NULL, "max one arg allowed");
		exit(1);
	}
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
	if (TESTER && !cleanup_data.input)
		main_exit(&cleanup_data, true, false);
	if (!ast)
		main_exit(&cleanup_data, full_exit_status(false) == true, false);
	while (1)
	{
		if (ast)
		{
			add_global_data(ast, &shared_data);
			ast->shared_data->cleanup_data = &cleanup_data;
			run_node(ast);
			main_exit(&cleanup_data, full_exit_status(false) == true, false);
		}
		ast = get_input(&cleanup_data);
		if (!ast)
			main_exit(&cleanup_data, full_exit_status(false) == true, false);
	}
	return (0);
}
