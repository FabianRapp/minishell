/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 18:05:59 by frapp             #+#    #+#             */
/*   Updated: 2024/03/25 01:32:54 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	signal_handler_ctrl_c(int signal, siginfo_t *info, void *data)
{
	(void)info;
	(void)data;
	(void)signal;
	rl_replace_line("", 0);
	rl_redisplay();
	rl_on_new_line();
	printf("\n");
}

void	signal_handler_ctrl_slash(int signal, siginfo_t *info, void *data)
{
	printf("%d\n", signal);
	(void)info;
	(void)data;
	(void)signal;
}

t_result	set_ctrl_slash(struct sigaction *sig)
{
	sigemptyset(&(sig->sa_mask));
	sig->sa_flags = SA_SIGINFO;
	sig->sa_sigaction = signal_handler_ctrl_slash;
	if (sigaction(3, sig, NULL) == -1)
	{
		printf("%s\n", strerror(errno));
		//! need to add the cleanup
		exit(errno);
		return (ERROR);
	}
	return (SUCCESS);
}

t_result	set_ctrl_c(struct sigaction *sig)
{
	sigemptyset(&(sig->sa_mask));
	sig->sa_flags = SA_SIGINFO;
	sig->sa_sigaction = signal_handler_ctrl_c;
	if (sigaction(SIGINT, sig, NULL) == -1)
	{
		printf("%s\n", strerror(errno));
		//! need to add the cleanup
		exit(errno);
		return (ERROR);
	}
	return (SUCCESS);
}
