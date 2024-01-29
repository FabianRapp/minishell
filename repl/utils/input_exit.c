/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:36:01 by frapp             #+#    #+#             */
/*   Updated: 2024/01/29 12:17:18 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_ast	*get_input(t_cleanup_data *cleanup_data)
{
	char	*input;
	t_ast	*ast;

	ft_printf("minishell-$: ");
	input = readline("");
	add_history(input);
	ast = parser(input);
	cleanup_data->input = input;
	cleanup_data->root = ast;
	return (ast);
}

void	main_cleanup(t_cleanup_data *data)
{
	if (data->root)
		free_ast(data->root);
	my_free((void **)&(data->input));
	//system("leaks minishell");
}
