/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:36:01 by frapp             #+#    #+#             */
/*   Updated: 2024/03/02 20:13:36 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_ast	*get_input(t_cleanup_data *cleanup_data)
{
	char	*input;
	t_ast	*ast;
	int		i;
	//int		std_out;

	// todo: error handeling
	//std_out = dup(WRITE);
	//dup2(2, WRITE);
	input = readline("minishell-$: ");
	//dup2(std_out, WRITE);
	if (!input)
		return (NULL);
	i = 0;
	while (input[i])
	{
		if (!ft_iswhitespace(input[i]))
		{
			add_history(input);
			ast = parser(input);
			if (ast)
			{
				cleanup_data->input = input;
				cleanup_data->root = ast;
			}
			else
				break ;
			return (ast);
		}
		i++;
	}
	free(input);
	return (NULL);
}

void	main_exit(t_cleanup_data *data, bool full_exit, t_env *env, int exit_status)
{
	bool	main_process;

	data->root->env->stop_execution = false;
	// if (!data || !data->root)
	// 	exit(exit_status);
	main_process = env->main_process;
	if (full_exit)
	{
		my_free((void **)&(data->input));
		if (data->root)
			free_ast(data->root);
		cleanup_fds();
		reset_stdio(RESET_STDIO_CLEAN);
		if (LEAK_CHECK)
			system("leaks minishell");
		check_fds(true);
		exit(exit_status);
	}
	else if (!full_exit)
	{
		my_free((void **)&(data->input));
		if (data->root)
			free_ast(data->root);
		cleanup_fds();
		if (LEAK_CHECK)
			system("leaks minishell");
		check_fds(false);
	}
	// else if (!main_process && full_exit)
	// {
	// 	exit(exit_status);
	// }
	// else if (!main_process)
	// {
	// 	exit(exit_status);
	// }
	else
	{
		my_free((void **)&(data->input));
		if (data->root)
			free_ast(data->root);
	}
	//system("leaks minishell");
}
