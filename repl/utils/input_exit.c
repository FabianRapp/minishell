/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:36:01 by frapp             #+#    #+#             */
/*   Updated: 2024/03/09 02:40:29 by frapp            ###   ########.fr       */
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
	input = readline("minishell-$: ");
	if (!input)
	{
		return (NULL);
	}
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

void	main_exit(t_cleanup_data *data, bool full_exit)
{
	wait_all_children();
	if (data->root && data->root->env)
		data->root->env->stop_execution = false;
	if (full_exit)
	{
		ft_free((void **)&(data->input));
		if (data->root)
			free_ast(data->root);
		cleanup_fds();
		if (LEAK_CHECK)
			system("leaks minishell");
		check_fds();
		exit(get_last_exit());
	}
	else if (!full_exit)
	{
		ft_free((void **)&(data->input));
		if (data->root)
			free_ast(data->root);
		data->root = NULL;
		cleanup_fds();
		if (LEAK_CHECK)
			system("leaks minishell");
		check_fds();
		errno = 0;
	}
}
