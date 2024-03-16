/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_sub_shell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 07:36:56 by frapp             #+#    #+#             */
/*   Updated: 2024/03/16 21:24:57 by frapp            ###   ########.fr       */
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

static t_ast	*init_sub_shell(t_ast *ast, char *input, t_env *sub_env, t_cleanup_data *cleanup_data)
{
	t_ast		*sub_ast;
	char		**env_list;
	char		**exp_list;

	ast->pid = fork();
	errno = 0;
	if (ast->pid)
	{
		// waitpid(ast->pid, &(ast->exit_status), 0);
		// ast->exit_status = WEXITSTATUS(ast->exit_status);
		// set_last_exit(ast->exit_status);
		// printf("val: %d\n", ast->exit_status);
		return (NULL);
	}
	env_list = ft_initialize_our_env(*(ast->envs));
	if (env_list == NULL)
		return (NULL);
	exp_list = ft_initialize_our_env(*(ast->envs));
	if (exp_list == NULL)
		return (ft_free_2darr(env_list), NULL);
	// if (!contains_non_white_spcace(input))
	// {
	// 	return (NULL);
	// }
	sub_ast = parser(input);
	if (!sub_ast)
	{
		main_exit(cleanup_data, true, false);
	}
	cleanup_data->input = input;
	cleanup_data->root = sub_ast;
	add_global_data(sub_ast, sub_env, &env_list, cleanup_data, &exp_list);
	if (mange_sub_shell_fds(ast) == ERROR)
		return (free_ast(sub_ast), exit(ast->exit_status), NULL);
	return (sub_ast);
}

// TODO: does every sub process command update the last exit?
static void	run_sub_shell(t_env sub_env, char *input, t_ast *ast)
{
	t_ast			*sub_ast;
	t_cleanup_data	cleanup_data;

	cleanup_data.root = NULL;
	cleanup_data.input = NULL;
	sub_ast = init_sub_shell(ast, input, &sub_env, &cleanup_data);
	if (sub_ast == NULL)
		return ;
	sub_ast->exit_status = ast->exit_status;
	run_node(sub_ast);
	if (sub_ast->exit_status == DEFAULT_EXIT_STATUS)
	{
		waitpid(sub_ast->pid, &(sub_ast->exit_status), 0);
		sub_ast->exit_status = WEXITSTATUS(sub_ast->exit_status);
	}
	wait_all_children(sub_ast);
	set_last_exit(sub_ast->exit_status);
	//sub_ast->exit_status = get_last_exit();
	ast->exit_status = sub_ast->exit_status;
	free_ast(sub_ast);
	//printf("%d\n", ast->exit_status);
	exit(ast->exit_status);
	return ;
}

// these error should be caught before
// lets make sure thers no segault just in case =)
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
			"syntax 111 error near unexpected token `)'");
		set_last_exit(2);
		return (ERROR);
	}
	return (SUCCESS);
}

void	create_sub_shell(t_ast *ast)
{
	if (ast->env->stop_execution && ast->exit_status == DEFAULT_EXIT_STATUS)
		ast->exit_status = 1;
	if (ast->exit_status != DEFAULT_EXIT_STATUS)
		return ;
	if (check_missing_input(ast->name->token->str_data) == ERROR)
	{
		ast->exit_status = 2;
		return ;
	}
	sub_shell_mode(SET_SUB_SHELL);
	run_sub_shell(*(ast->env), ast->name->token->str_data, ast);
	sub_shell_mode(UNSET_SUB_SHELL);
	if (ast->pid == -1)
	{
		ast->exit_status = errno;
		print_error(true, NULL, NULL, strerror(errno));
	}
}
