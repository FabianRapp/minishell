/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:36:01 by frapp             #+#    #+#             */
/*   Updated: 2024/03/06 07:11:52 by frapp            ###   ########.fr       */
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
	cleanup_data->root = NULL;
	cleanup_data->input = NULL;
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
			{
				break ;
			}
			return (ast);
		}
		i++;
	}
	free(input);
	return (NULL);
}

void	main_exit(t_cleanup_data *data, bool full_exit)
{
	if (data->root && data->root->env)
		data->root->env->stop_execution = false;
	if (full_exit)
	{
		my_free((void **)&(data->input));
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
		my_free((void **)&(data->input));
		if (data->root)
			free_ast(data->root);
		data->root = NULL;
		cleanup_fds();
		if (LEAK_CHECK)
			system("leaks minishell");
		check_fds();
	}
}
