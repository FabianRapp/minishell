/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:36:01 by frapp             #+#    #+#             */
/*   Updated: 2024/03/26 18:54:00 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

// t_ast	*handle_manunal_input(char **av, t_cleanup_data *cleanup_data)
// {
// 	char	*input;
// 	t_ast	*ast;

// 	cleanup_data->root = NULL;
// 	cleanup_data->input = NULL;
// 	input = ft_strdup(av[1]);
// 	if (!input)
// 		return (NULL);
// 	if (!contains_non_white_spcace(input))
// 	{
// 		return (free(input), NULL);
// 	}
// 	add_history(input);
// 	ast = parser(input);
// 	if (ast)
// 	{
// 		cleanup_data->input = input;
// 		cleanup_data->root = ast;
// 		return (ast);
// 	}
// 	return (NULL);
// }

t_ast	*get_input(t_cleanup_data *cleanup_data)
{
	char	*input;
	t_ast	*ast;

	cleanup_data->root = NULL;
	cleanup_data->input = NULL;
	input = ft_read_line("minishell-$: ");
	if (!input)
	{
		if (!TESTER)
			ft_fprintf(2, "exit\n");
		main_exit(cleanup_data, true, true);
	}
	errno = 0;
	if (!contains_non_white_spcace(input))
		return (free(input), NULL);
	add_history(input);
	ast = parser(input);
	errno = 0;
	if (!ast)
		return (free(input), NULL);
	cleanup_data->input = input;
	cleanup_data->root = ast;
	return (ast);
}

static void	free_and_exit(t_shared_data	*shared_data, bool full_exit)
{
	if (full_exit)
	{
		if (shared_data->env_exp)
			ft_free_2darr(*(shared_data->env_exp));
		if (shared_data->envs)
			ft_free_2darr(*(shared_data->envs));
		if (LEAK_CHECK)
			system("leaks minishell");
		rl_clear_history();
		exit(get_last_exit());
	}
	if (LEAK_CHECK)
		system("leaks minishell");
	errno = 0;
}

void	main_exit(t_cleanup_data *data, bool full_exit, bool ft_exit_call)
{
	t_shared_data	*shared_data;

	shared_data = data->shared_data;
	if (data && data->root && !ft_exit_call
		&& data->root->exit_status == DEFAULT_EXIT_STATUS)
	{
		if (data->root && data->root->pid > 0)
			waitpid(data->root->pid, &(data->root->exit_status), 0);
		data->root->exit_status = WEXITSTATUS(data->root->exit_status);
		set_last_exit(data->root->exit_status);
	}
	wait_all_children(data->root);
	if (data->root && data->root->shared_data)
		data->root->shared_data->stop_execution = false;
	free(data->input);
	if (data->root)
		free_ast(data->root);
	data->root = NULL;
	cleanup_fds();
	free_and_exit(shared_data, full_exit);
}
