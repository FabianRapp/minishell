/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 09:58:09 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 21:48:15 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static void	handler_ctrl_c_heredoc(int signal, siginfo_t *info, void *data)
{
	struct termios	base_term;
	struct termios	cur_term;

	(void)info;
	(void)data;
	(void)signal;
	tcgetattr(0, &cur_term);
	get_base_term(false, &base_term);
	base_term.c_lflag &= ECHOCTL;
	tcsetattr(0, TCSANOW, &base_term);
	here_doc_exit_state(true, true);
	set_last_exit(130);
	tcsetattr(0, TCSANOW, &cur_term);
}

t_result	set_ctrl_c_heredoc(void)
{
	struct sigaction	sig;

	sigemptyset(&(sig.sa_mask));
	sig.sa_flags = SA_SIGINFO;
	sig.sa_sigaction = handler_ctrl_c_heredoc;
	if (sigaction(SIGINT, &sig, NULL) == -1)
	{
		print_error(true, NULL, NULL, strerror(errno));
		set_last_exit(errno);
		full_exit_status(true);
		return (ERROR);
	}
	set_sig_do_nothing(SIGQUIT);
	return (SUCCESS);
}

t_result	set_signals_heredoc_parent(void)
{
	struct sigaction	sig;

	set_sig_do_nothing(SIGQUIT);
	sigemptyset(&(sig.sa_mask));
	sig.sa_flags = SA_SIGINFO;
	sig.sa_sigaction = handler_ctrl_c_heredoc;
	if (sigaction(SIGINT, &sig, NULL) == -1)
	{
		print_error(true, NULL, NULL, strerror(errno));
		set_last_exit(errno);
		full_exit_status(true);
		return (ERROR);
	}
	set_sig_do_nothing(SIGQUIT);
	return (SUCCESS);
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
