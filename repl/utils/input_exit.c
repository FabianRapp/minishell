/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:36:01 by frapp             #+#    #+#             */
/*   Updated: 2024/02/01 16:02:00 by frapp            ###   ########.fr       */
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

void	main_cleanup(t_cleanup_data *data, bool full_exit, bool main_process)
{
	if (!data)
		return ;
	//reset_stdio(data->root);
	if (data->root && main_process)
	{
		if (full_exit && data->root->env)
			free_env(data->root->env);
		if (data->root)
			free_ast(data->root);
	}
	my_free((void **)&(data->input));
	//system("leaks minishell");
}
