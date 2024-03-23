/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:36:01 by frapp             #+#    #+#             */
/*   Updated: 2024/03/24 00:07:59 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

// if a line is passed via av
t_ast	*handle_manunal_input(char **av, t_cleanup_data *cleanup_data)
{
	char	*input;
	t_ast	*ast;

	cleanup_data->root = NULL;
	cleanup_data->input = NULL;
	input = ft_strdup(av[1]);
	if (!input)
		return (NULL);
	if (!contains_non_white_spcace(input))
	{
		return (free(input), NULL);
	}
	add_history(input);
	ast = parser(input);
	if (ast)
	{
		cleanup_data->input = input;
		cleanup_data->root = ast;
		return (ast);
	}
	return (NULL);
}

t_ast	*get_input(t_cleanup_data *cleanup_data)
{
	char	*input;
	t_ast	*ast;

	cleanup_data->root = NULL;
	cleanup_data->input = NULL;
	//set_ctrl_c(&(cleanup_data->shared_data->sig_set));
	input = ft_read_line("minishell-$: ");
	//signal(SIGINT, SIG_DFL);
	//input = readline("minishell-$: ");
	
	if (!input)
	{
		if (!TESTER)
			ft_fprintf(2, "exit\n");
		main_exit(cleanup_data, true, true);
	}
	if (!contains_non_white_spcace(input))
		return (free(input), NULL);
	add_history(input);
	ast = parser(input);
	if (ast)
	{
		cleanup_data->input = input;
		cleanup_data->root = ast;
		return (ast);
	}
	else
		free(input);
	//exit(get_last_exit());
	return (NULL);
}

void	main_exit(t_cleanup_data *data, bool full_exit, bool ft_exit_call)
{
	t_shared_data	*shared_data;
	// if (!data)
	// 	printf("no cleanup data\n");
	// else if (!data->root)
	// 	printf("no data root\n");
	// if (sub_shell_mode(GET_SUB_SHELL_MODE) == true)
	// 	printf("in sub mode: %d/%d\n", data->root->exit_status, get_last_exit());
	// else
	// 	printf("not sub mode: %d/%d(root/last)\n", data->root->exit_status, get_last_exit());
	shared_data = data->shared_data;
	if (data && data->root && !ft_exit_call && data->root->exit_status == DEFAULT_EXIT_STATUS)
	{
		if (data->root && data->root->pid > 0)
			waitpid(data->root->pid, &(data->root->exit_status), 0);
		data->root->exit_status = WEXITSTATUS(data->root->exit_status);
		set_last_exit(data->root->exit_status);
	}
	wait_all_children(data->root);
	// if (!data)
	// 	printf("no cleanup data\n");
	// else if (!data->root)
	// 	printf("no data root\n");
	// if (sub_shell_mode(GET_SUB_SHELL_MODE) == true)
	// 	printf("in sub mode: %d/%d\n", data->root->exit_status, get_last_exit());
	// else
	// 	printf("not sub mode: %d/%d(root/last)\n", data->root->exit_status, get_last_exit());
	if (data->root && data->root->shared_data)
		data->root->shared_data->stop_execution = false;
	free(data->input);
	if (data->root)
		free_ast(data->root);
	data->root = NULL;
	cleanup_fds();

	//check_fds();
	if (full_exit)
	{
		if (shared_data->env_exp)
			ft_free_2darr(*(shared_data->env_exp));
		if (shared_data->envs)
			ft_free_2darr(*(shared_data->envs));
		if (LEAK_CHECK)
			system("leaks minishell");
		exit(get_last_exit());
	}
	if (LEAK_CHECK)
		system("leaks minishell");
	errno = 0;
}
