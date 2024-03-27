/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 18:05:59 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 18:09:40 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

bool	redisplay_prompt(bool set_state, bool new_state)
{
	static bool		redisplay_prompt_state = false;

	if (set_state)
		redisplay_prompt_state = new_state;
	return (redisplay_prompt_state);
}

t_result	set_sig_do_nothing(int signal)
{
	struct sigaction	sig;

	sigemptyset(&(sig.sa_mask));
	sig.sa_flags = 0;
	sig.sa_handler = SIG_IGN;
	if (sigaction(signal, &sig, NULL) == -1)
	{
		print_error(true, NULL, NULL, strerror(errno));
		set_last_exit(errno);
		full_exit_status(true);
		return (ERROR);
	}
	return (SUCCESS);
}

static void	signal_handler_ctrl_c(int signal, siginfo_t *info, void *data)
{
	(void)info;
	(void)data;
	(void)signal;
	here_doc_exit_state(true, true);
	set_last_exit(130);
	rl_replace_line("", 0);
	rl_on_new_line();
	ft_printf("\n");
	if (redisplay_prompt(false, false))
		rl_redisplay();
}

// static void	signal_handler_ctrl_c2(int signal, siginfo_t *info, void *data)
// {
// 	(void)info;
// 	(void)data;
// 	(void)signal;
// 	here_doc_exit_state(true, true);
// 	set_last_exit(130);
// 	rl_replace_line("123123123123", 0);
// 	//rl_on_new_line();
// 	ft_printf("123123\n");
// 	// rl_replace_line("", 0);
// 	rl_on_new_line();
// 	ft_printf("test\n");
// 	if (redisplay_prompt(false, false))
// 		rl_redisplay();
// }

t_result	set_ctrl_c(int nl_count)
{
	struct sigaction	sig;

	(void)nl_count;
	sigemptyset(&(sig.sa_mask));
	sig.sa_flags = SA_SIGINFO;
	//if (nl_count == 1)
		sig.sa_sigaction = signal_handler_ctrl_c;
	// else if (nl_count == 2)
	// 	sig.sa_sigaction = signal_handler_ctrl_c2;
	if (sigaction(SIGINT, &sig, NULL) == -1)
	{
		print_error(true, NULL, NULL, strerror(errno));
		set_last_exit(errno);
		full_exit_status(true);
		return (ERROR);
	}
	return (SUCCESS);
}

void	set_signals(void)
{
	set_ctrl_c(1);
	set_sig_do_nothing(SIGQUIT);
}