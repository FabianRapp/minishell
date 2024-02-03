/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:36:01 by frapp             #+#    #+#             */
/*   Updated: 2024/02/02 09:13:59 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_ast	*get_input(t_cleanup_data *cleanup_data)
{
	char	*input;
	t_ast	*ast;

	input = readline("minishell-$: ");
	add_history(input);
	ast = parser(input);
	cleanup_data->input = input;
	cleanup_data->root = ast;
	return (ast);
}

void	main_exit(t_cleanup_data *data, bool full_exit, t_env *env, int exit_status)
{
	bool	main_process;

	if (!data || !data->root)
		exit(exit_status);
	main_process = env->main_process;
	//reset_stdio(data->root);
	if (main_process && full_exit)
	{
		my_free((void **)&(data->input));
		if (env)
			free_env(env);
		if (data->root)
			free_ast(data->root);
	}
	else if (!main_process && full_exit)
	{
		free_env(env);
	}
	else if (!main_process)
	{
		exit(exit_status);
		if (env)
			free_env(env);
	}
	else
	{
		my_free((void **)&(data->input));
		if (data->root)
			free_ast(data->root);
	}
	//system("leaks minishell");
}
