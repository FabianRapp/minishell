/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 18:05:59 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 22:52:44 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	get_base_term(bool set, struct termios *new_term)
{
	static struct termios	base_set;

	if (set)
		base_set = *new_term;
	*new_term = base_set;
}

bool	redisplay_prompt(bool set_state, bool new_state)
{
	static bool		redisplay_prompt_state = false;

	if (set_state)
		redisplay_prompt_state = new_state;
	return (redisplay_prompt_state);
}

static void	signal_handler_ctrl_c(int signal, siginfo_t *info, void *data)
{
	struct termios	base_term;
	struct termios	cur_term;

	(void)info;
	(void)data;
	(void)signal;
	if (isatty(0))
		tcgetattr(0, &cur_term);
	get_base_term(false, &base_term);
	if (isatty(0))
		tcsetattr(0, TCSANOW, &base_term);
	here_doc_exit_state(true, true);
	set_last_exit(130);
	rl_replace_line("", 0);
	rl_on_new_line();
	ft_printf("\n");
	if (redisplay_prompt(false, false))
		rl_redisplay();
	if (isatty(0))
		tcsetattr(0, TCSANOW, &cur_term);
}

t_result	set_ctrl_c(int nl_count)
{
	struct sigaction	sig;

	(void)nl_count;
	sigemptyset(&(sig.sa_mask));
	sig.sa_flags = SA_SIGINFO;
	sig.sa_sigaction = signal_handler_ctrl_c;
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
