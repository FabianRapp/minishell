/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 18:05:59 by frapp             #+#    #+#             */
/*   Updated: 2024/03/25 05:12:43 by frapp            ###   ########.fr       */
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



// void	signal_handler_ctrl_slash(int signal)
// {
// 	printf("got SIGQUIT signal\n");
// 	(void)info;
// 	(void)data;
// 	(void)signal;
// }

t_result	set_ctrl_slash(void)
{
	struct sigaction	sig;

	sigemptyset(&(sig.sa_mask));
	sig.sa_flags = 0;
	sig.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sig, NULL) == -1)
	{
		print_error(true, NULL, NULL, strerror(errno));
		set_last_exit(errno);
		full_exit_status(true);
		return (ERROR);
	}
	return (SUCCESS);
}

void	signal_handler_ctrl_c(int signal, siginfo_t *info, void *data)
{
	(void)info;
	(void)data;
	(void)signal;
	set_last_exit(130);
	rl_replace_line("", 0);
	rl_on_new_line();
	printf("\n");
	if (redisplay_prompt(false, false))
		rl_redisplay();
}

t_result	set_ctrl_c(void)
{
	struct sigaction	sig;

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
	set_ctrl_slash();
	set_ctrl_c();
}

void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	reset_terminal_settings(void)
{
	struct termios	terminal;

	if (tcgetattr(STDIN_FILENO, &terminal) == -1)
	{	
		print_error(true, NULL, NULL, strerror(errno));
		set_last_exit(errno);
		full_exit_status(true);
		return ;
	}
	terminal.c_lflag |= ECHO;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &terminal) == -1)
	{	
		print_error(true, NULL, NULL, strerror(errno));
		set_last_exit(errno);
		full_exit_status(true);
		return ;
	}
}

void	init_terminal_settings(void)
{
	struct termios	terminal;

	if (tcgetattr(STDIN_FILENO, &terminal) == -1)
	{
		print_error(true, NULL, NULL, strerror(errno));
		set_last_exit(errno);
		full_exit_status(true);
		return ;
	}
	terminal.c_lflag &= ~ECHO;
	terminal.c_lflag &= ~ISIG;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &terminal) == -1)
	{	
		reset_terminal_settings();
		print_error(true, NULL, NULL, strerror(errno));
		set_last_exit(errno);
		full_exit_status(true);
		return ;
	}
}
