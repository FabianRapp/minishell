/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_state.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 19:27:17 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 22:53:13 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static int	last_exit_data(t_status_handler data)
{
	static int	last_exit_status = 0;

	if (data.set)
		last_exit_status = data.val;
	return (last_exit_status);
}

void	set_last_exit(int exit_status)
{
	t_status_handler	data;

	data.set = true;
	data.val = exit_status;
	last_exit_data(data);
}

char	*get_last_exit_str(void)
{
	t_status_handler	data;

	data.set = false;
	return (ft_itoa(last_exit_data(data)));
}

int	get_last_exit(void)
{
	t_status_handler	data;

	data.set = false;
	return (last_exit_data(data));
}

t_result	ft_cur_exit(t_ast *ast, int exit_value)
{
	ast->exit_status = exit_value;
	set_last_exit(exit_value);
	return (SUCCESS);
}
