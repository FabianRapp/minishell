/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 18:05:59 by frapp             #+#    #+#             */
/*   Updated: 2024/03/21 18:24:51 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	signal_handler_ctrl_c(int signal, siginfo_t *info, void *data)
{
	(void)info;
	(void)data;
	(void)signal;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	// printf("%s", SHELL_PROMPT);
}

t_result	set_ctrl_c(struct sigaction *sig)
{
	sigemptyset(&(sig->sa_mask));
	sig->sa_flags = SA_SIGINFO;
	sig->sa_sigaction = signal_handler_ctrl_c;
	if (sigaction(SIGINT, sig, NULL) == -1)
	{
		printf("%s\n", strerror(errno));
		exit(errno);
		return (ERROR);
	}
	return (SUCCESS);
}

void	reset_terminal()
{
}

void	set_terminal()
{
}
