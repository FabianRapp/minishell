/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_parent_coms.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 15:01:17 by frapp             #+#    #+#             */
/*   Updated: 2024/02/15 05:03:20 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/signals.h"

void	child_exit_handler(int sig)
{
	int	status;

	while (waitpid(-1, &status, WNOHANG) > 0)
	{
		if (WIFEXITED(status))
			errno = WEXITSTATUS(status);
	}
	(void) sig;
}
