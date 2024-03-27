/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_state.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 00:30:16 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 08:57:57 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

bool	sub_shell_mode(int flag)
{
	static bool	sub_shell_mode_state = false;

	if (flag == SET_SUB_SHELL)
		sub_shell_mode_state = true;
	else if (flag == UNSET_SUB_SHELL)
		sub_shell_mode_state = false;
	return (sub_shell_mode_state);
}

// call this one time for each line read from std in,
// dont call this otherwise
int	line_counter(void)
{
	static int	line_count = -1;

	line_count++;
	return (line_count);
}

bool	here_doc_exit_state(bool change_state, bool new_state)
{
	static bool	here_doc_exit = false;

	if (change_state)
		here_doc_exit = new_state;
	return (here_doc_exit);
}

bool	ignore_empty_line(bool set_next_to_ignore)
{
	static bool	ignore_next_emtpy = false;
	bool		return_val;

	return_val = ignore_next_emtpy;
	ignore_next_emtpy = set_next_to_ignore;
	return (return_val);
}
