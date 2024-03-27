/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:36:01 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 21:54:51 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static char	*get_input_util(char *input, t_cleanup_data *cleanup_data)
{
	if (!input && !ignore_empty_line(false))
	{
		if (!TESTER)
			ft_fprintf(2, "exit\n");
		main_exit(cleanup_data, true, true);
	}
	errno = 0;
	if (!contains_non_white_spcace(input))
		return (free(input), NULL);
	add_history(input);
	return (input);
}

t_ast	*get_input(t_cleanup_data *cleanup_data)
{
	char			*input;
	t_ast			*ast;
	struct termios	term_settings;

	cleanup_data->root = NULL;
	cleanup_data->input = NULL;
	set_signals();
	input = ft_read_line("minishell-$: ");
	set_sig_do_nothing(SIGINT);
	if (!get_input_util(input, cleanup_data))
		return (NULL);
	term_settings = cleanup_data->shared_data->base_term_settings;
	term_settings.c_lflag &= ~ECHOCTL;
	if (isatty(0) && tcsetattr(0, TCSANOW, &term_settings) == -1)
		return (free(input), NULL);
	ast = parser(input);
	tcsetattr(0, TCSANOW, &cleanup_data->shared_data->base_term_settings);
	errno = 0;
	if (!ast)
		return (free(input), NULL);
	cleanup_data->input = input;
	cleanup_data->root = ast;
	return (ast);
}

static void	free_and_exit(t_shared_data	*shared_data, bool full_exit)
{
	if (full_exit)
	{
		if (shared_data->env_exp)
			ft_free_2darr(*(shared_data->env_exp));
		if (shared_data->envs)
			ft_free_2darr(*(shared_data->envs));
		if (full_exit_status(false) && !TESTER)
			ft_fprintf(2, "exit\n");
		rl_clear_history();
		wait_all_children(NULL);
		exit(get_last_exit());
	}
	errno = 0;
}

void	main_exit(t_cleanup_data *data, bool full_exit, bool ft_exit_call)
{
	t_shared_data	*shared_data;

	tcsetattr(0, TCSANOW, &data->shared_data->base_term_settings);
	shared_data = data->shared_data;
	if (data && data->root && !ft_exit_call
		&& data->root->exit_status == DEFAULT_EXIT_STATUS)
	{
		if (data->root && data->root->pid > 0)
			waitpid(data->root->pid, &(data->root->exit_status), 0);
		data->root->exit_status = WEXITSTATUS(data->root->exit_status);
		set_last_exit(data->root->exit_status);
	}
	if (data->root && data->root->shared_data)
		data->root->shared_data->stop_execution = false;
	free(data->input);
	if (data->root)
		free_ast(data->root);
	data->root = NULL;
	cleanup_fds();
	free_and_exit(shared_data, full_exit);
}
