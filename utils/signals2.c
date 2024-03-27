/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 09:58:09 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 08:50:10 by frapp            ###   ########.fr       */
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
	//t_here_doc_child_data	*vars;

	(void)info;
	(void)data;
	(void)signal;
	here_doc_exit_state(true, true);
	// vars = heredoc_chil_data_state(NULL);
	// free(vars->line);
	// free(vars->termination);
	// if (vars->fd[WRITE] != INIT_VAL)
	// 	close(vars->fd[WRITE]);
	// close(vars->fd[READ]);
	//exit(130);
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
	return (SUCCESS);
}

// void	reset_terminal_settings(void)
// {
// 	struct termios	terminal;

// 	if (tcgetattr(STDIN_FILENO, &terminal) == -1)
// 	{
// 		print_error(true, NULL, NULL, strerror(errno));
// 		set_last_exit(errno);
// 		full_exit_status(true);
// 		return ;
// 	}
// 	terminal.c_lflag |= ECHO;
// 	if (tcsetattr(STDIN_FILENO, TCSANOW, &terminal) == -1)
// 	{
// 		print_error(true, NULL, NULL, strerror(errno));
// 		set_last_exit(errno);
// 		full_exit_status(true);
// 		return ;
// 	}
// }

// void	init_terminal_settings(void)
// {
// 	struct termios	terminal;

// 	if (tcgetattr(STDIN_FILENO, &terminal) == -1)
// 	{
// 		print_error(true, NULL, NULL, strerror(errno));
// 		set_last_exit(errno);
// 		full_exit_status(true);
// 		return ;
// 	}
// 	terminal.c_lflag &= ~ECHO;
// 	terminal.c_lflag &= ~ISIG;
// 	if (tcsetattr(STDIN_FILENO, TCSANOW, &terminal) == -1)
// 	{
// 		reset_terminal_settings();
// 		print_error(true, NULL, NULL, strerror(errno));
// 		set_last_exit(errno);
// 		full_exit_status(true);
// 		return ;
// 	}
// }
