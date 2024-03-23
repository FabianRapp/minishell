/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 19:27:17 by frapp             #+#    #+#             */
/*   Updated: 2024/03/06 00:33:40 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../headers/utils.h"

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

bool	full_exit_status(bool set_full_exit)
{
	static bool	full_exit = false;

	if (set_full_exit)
		full_exit = true;
	return (full_exit);
}

bool	sub_shell_mode(int flag)
{
	static bool	sub_shell_mode_state = false;

	if (flag == SET_SUB_SHELL)
		sub_shell_mode_state = true;
	else if (flag == UNSET_SUB_SHELL)
		sub_shell_mode_state = false;
	return (sub_shell_mode_state);
}