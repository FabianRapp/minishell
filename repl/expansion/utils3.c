/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 19:27:17 by frapp             #+#    #+#             */
/*   Updated: 2024/02/09 19:48:31 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

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

char	*get_last_exit(void)
{
	t_status_handler	data;

	data.set = false;
	return (ft_itoa(last_exit_data(data)));
}
