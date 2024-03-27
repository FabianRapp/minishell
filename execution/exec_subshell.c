/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 07:36:56 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 13:43:48 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static t_result	mange_sub_shell_fds(t_ast *ast)
{
	if (resolve_redirs(ast) == ERROR)
	{
		cleanup_fds();
		return (ERROR);
	}
	redir_fds();
	if (errno)
	{
		print_error(true, NULL, NULL, strerror(errno));
		ast->exit_status = errno;
		set_last_exit(errno);
		return (ERROR);
	}
	free(io_data(-1, NULL));
	io_data(CLEANUP_FDS, NULL);
	if (errno)
	{
		print_error(true, NULL, NULL, strerror(errno));
		ast->exit_status = errno;
		set_last_exit(errno);
		return (ERROR);
	}
	return (SUCCESS);
}

static t_ast	*init_sub_shell(t_ast *ast, char *input,
	t_shared_data *shared_sub_vars, t_cleanup_data *sub_cleanup_data)
{
	t_ast		*sub_ast;

	if (check_brackets(ast, input) == ERROR)
		return (NULL);
	sub_cleanup_data->shared_data = shared_sub_vars;
	ast->pid = fork();
	errno = 0;
	if (ast->pid)
		return (NULL);
	*shared_sub_vars = *(ast->shared_data);
	sub_ast = parser(input);
	if (!sub_ast)
		main_exit(sub_cleanup_data, true, false);
	sub_cleanup_data->input = input;
	sub_cleanup_data->root = sub_ast;
	shared_sub_vars->cleanup_data = sub_cleanup_data;
	add_global_data(sub_ast, shared_sub_vars);
	if (mange_sub_shell_fds(ast) == ERROR)
		return (free_ast(sub_ast), exit(ast->exit_status), NULL);
	return (sub_ast);
}

static void	run_sub_shell(t_shared_data shared_sub_vars,
	char *input, t_ast *ast)
{
	t_ast			*sub_ast;
	t_cleanup_data	sub_cleanup_data;

	sub_cleanup_data.root = NULL;
	sub_cleanup_data.input = NULL;
	sub_ast = init_sub_shell(ast, input, &shared_sub_vars,
			&sub_cleanup_data);
	if (sub_ast == NULL)
		return ;
	sub_ast->exit_status = ast->exit_status;
	run_node(sub_ast);
	if (sub_ast->exit_status == DEFAULT_EXIT_STATUS)
	{
		waitpid(sub_ast->pid, &(sub_ast->exit_status), 0);
		sub_ast->exit_status = WEXITSTATUS(sub_ast->exit_status);
	}
	set_last_exit(sub_ast->exit_status);
	ast->exit_status = sub_ast->exit_status;
	free_ast(sub_ast);
	exit(ast->exit_status);
	return ;
}

static t_result	check_missing_input(char *input)
{
	if (!input)
	{
		print_error(true, NULL, NULL, "Error");
		set_last_exit(2);
		return (ERROR);
	}
	if (!*input)
	{
		print_error(true, NULL, NULL,
			"syntax error near unexpected token `)'");
		set_last_exit(2);
		return (ERROR);
	}
	return (SUCCESS);
}

void	create_sub_shell(t_ast *ast)
{
	if (ast->shared_data->stop_execution && ast->exit_status
		== DEFAULT_EXIT_STATUS)
		ast->exit_status = 1;
	if (ast->exit_status != DEFAULT_EXIT_STATUS)
		return ;
	if (check_missing_input(ast->name->token->str_data) == ERROR)
	{
		ast->exit_status = get_last_exit();
		return ;
	}
	sub_shell_mode(SET_SUB_SHELL);
	run_sub_shell(*(ast->shared_data), ast->name->token->str_data, ast);
	sub_shell_mode(UNSET_SUB_SHELL);
	if (ast->pid == -1)
	{
		ast->exit_status = errno;
		print_error(true, NULL, NULL, strerror(errno));
	}
}
