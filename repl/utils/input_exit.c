/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:36:01 by frapp             #+#    #+#             */
/*   Updated: 2024/03/10 11:18:48 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

// if a line is passed via av
t_ast	*handle_manunal_input(char **av, t_cleanup_data *cleanup_data)
{
	char	*input;
	t_ast	*ast;

	cleanup_data->root = NULL;
	cleanup_data->input = NULL;
	input = ft_strdup(av[1]);
	if (!input)
		return (NULL);
	if (!contains_non_white_spcace(input))
	{
		return (free(input), NULL);
	}
	add_history(input);
	ast = parser(input);
	if (ast)
	{
		cleanup_data->input = input;
		cleanup_data->root = ast;
		return (ast);
	}
	return (NULL);
}

t_ast	*get_input(t_cleanup_data *cleanup_data)
{
	char	*input;
	t_ast	*ast;
	char	*temp;

	cleanup_data->root = NULL;
	cleanup_data->input = NULL;
	if (cleanup_data->in_arr && !cleanup_data->in_arr[cleanup_data->input_i])
	{
		free_str_ar(cleanup_data->in_arr);
		cleanup_data->in_arr = NULL;
		cleanup_data->input_i = 0;
	}
	if (!cleanup_data->in_arr)
	{
		if (!isatty(fileno(stdin)))
		{
			input = get_next_line(fileno(stdin));
			temp = input;
			input = ft_strtrim(input, "\n");
			free(temp);
		}
		else
			input = readline("minishell-$: ");
		cleanup_data->in_arr = ft_split(input, '\n');
		if (!cleanup_data->in_arr)
		{
		}
	}
	input = cleanup_data->in_arr[cleanup_data->input_i];
	cleanup_data->input_i += 1;
	if (!input)
	{
		free_str_ar(cleanup_data->in_arr);
		cleanup_data->in_arr = NULL;
		//exit(get_last_exit());
		//if (TESTER)
			//exit(get_last_exit());
		return (NULL);
	}
	if (!contains_non_white_spcace(input))
	{
		//if (TESTER)
			//exit(get_last_exit());
		return (free(input), NULL);
	}
	add_history(input);
	ast = parser(input);
	if (ast)
	{
		//cleanup_data->input = input;
		cleanup_data->root = ast;
		return (ast);
	}
	//exit(get_last_exit());
	return (NULL);
}

void	main_exit(t_cleanup_data *data, bool full_exit, bool ft_exit_call)
{
	// if (!data)
	// 	printf("no cleanup data\n");
	// else if (!data->root)
	// 	printf("no data root\n");
	// if (sub_shell_mode(GET_SUB_SHELL_MODE) == true)
	// 	printf("in sub mode: %d/%d\n", data->root->exit_status, get_last_exit());
	// else
	// 	printf("not sub mode: %d/%d(root/last)\n", data->root->exit_status, get_last_exit());
	if (data && data->root && !ft_exit_call && data->root->exit_status == DEFAULT_EXIT_STATUS)
	{
		if (data->root && data->root->pid > 0)
			waitpid(data->root->pid, &(data->root->exit_status), 0);
		data->root->exit_status = WEXITSTATUS(data->root->exit_status);
		set_last_exit(data->root->exit_status);
	}
	wait_all_children();
	// if (!data)
	// 	printf("no cleanup data\n");
	// else if (!data->root)
	// 	printf("no data root\n");
	// if (sub_shell_mode(GET_SUB_SHELL_MODE) == true)
	// 	printf("in sub mode: %d/%d\n", data->root->exit_status, get_last_exit());
	// else
	// 	printf("not sub mode: %d/%d(root/last)\n", data->root->exit_status, get_last_exit());
	if (data->root && data->root->env)
		data->root->env->stop_execution = false;
	if (full_exit)
	{
		free_str_ar(data->in_arr);
		//ft_free((void **)&(data->input));
		if (data->root)
			free_ast(data->root);
		cleanup_fds();
		if (LEAK_CHECK)
			system("leaks minishell");
		//check_fds();
		exit(get_last_exit());
	}
	else if (!full_exit)
	{
		//ft_free((void **)&(data->input));
		if (data->root)
			free_ast(data->root);
		data->root = NULL;
		cleanup_fds();
		if (LEAK_CHECK)
			system("leaks minishell");
		//check_fds();
		errno = 0;
	}
}
