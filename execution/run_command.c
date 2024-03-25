/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 06:04:52 by frapp             #+#    #+#             */
/*   Updated: 2024/03/25 10:31:07 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parser.h"

void	update_underscre_var(t_ast *ast)
{
	char	*update_val;
	t_arg	*cur_arg;

	update_val = NULL;
	if (ast->name)
		update_val = ast->name->token->str_data;
	cur_arg = ast->arg;
	while (cur_arg && cur_arg->next)
	{
		cur_arg = cur_arg->next;
	}
	if (cur_arg)
		update_val = cur_arg->name->token->str_data;
	if (update_val)
		ft_update_env("_", update_val, *(ast->shared_data->envs));
}

bool	check_edgecases(t_ast *ast)
{
	if (!ast->name || ast->name->token->type == DUMMY_COMMAND)
	{
		ast->exit_status = 0;
		return (true);
	}
	if (!TESTER)
		update_underscre_var(ast);
	if (!ast->dont_run_buildins && ft_builtin_control(ast) == true)
	{
		return (true);
	}
	return (false);
}

void	init_child_data(t_child_data *data, t_ast *ast)
{
	data->argv = NULL;
	data->path = NULL;
	data->command_name = NULL;
	data->argv = ft_calloc(count_args(ast->arg) + 2, sizeof (char *const));
	if (!data->argv)
	{
		ast->exit_status = errno;
		return ;
	}
	data->command_name = ast->name->token->str_data;
	data->path = find_path(ast, data->command_name, "PATH");
	data->command_name = NULL;
	if (!data->path)
	{
		ast->exit_status = get_last_exit();
		return ;
	}
	if (ast->exit_status != DEFAULT_EXIT_STATUS)
		return ;
	data->argv[0] = extract_command_name(data->path);
	data->command_name = extract_command_name(data->path);
	if (errno)
		set_errno_as_exit(ast, false);
	else
		fill_args(ast, data->argv + 1, ARGS);
}

void	free_child_data(t_child_data *data)
{
	ft_free((void **)&(data->argv[0]));
	free(data->command_name);
	free(data->argv[0]);
	free(data->argv);
	free(data->path);
}

int	run_command_node(t_ast *ast)
{
	t_child_data	data;

	if (check_edgecases(ast))
		return (1);
	init_child_data(&data, ast);
	if (!data.path || ast->exit_status != DEFAULT_EXIT_STATUS)
		return (free_child_data(&data), 1);
	ast->pid = fork();
	errno = 0;
	if (ast->pid == -1)
		return (set_errno_as_exit(ast, true), free_child_data(&data), 0);
	if (ast->pid != 0)
		return (free_child_data(&data), 1);
	reset_signals();
	ft_close(&(ast->fd_to_close));
	ft_close(&(ast->fd_to_close_write));
	ft_close(&(ast->fd_to_close_read));
	if (ast->shared_data->envs)
		execve(data.path, data.argv, *(ast->shared_data->envs));
	else
		execve(data.path, data.argv, NULL);
	print_error(true, data.command_name, NULL, strerror(errno));
	return (exit(errno), 0);
}
