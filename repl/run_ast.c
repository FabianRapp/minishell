/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_ast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 12:08:53 by frapp             #+#    #+#             */
/*   Updated: 2024/03/11 15:42:52 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

// TODO errors
void	ft_pipe(t_ast *ast)
{
	int			pipe_fd[2];
	int			base_fd[2];

	if (ast->env->stop_execution)
	{
		ast->exit_status = 1;
		return ;
	}
	base_fd[READ] = dup(READ);
	base_fd[WRITE] = dup(WRITE);
	pipe(pipe_fd);
	dup2(pipe_fd[WRITE], WRITE);
	run_node(ast->left);
	dup2(base_fd[WRITE], WRITE);
	close(base_fd[WRITE]);
	close(pipe_fd[WRITE]);
	ast->env->stop_execution = false;
	dup2(pipe_fd[READ], READ);
	run_node(ast->right);
	dup2(base_fd[READ], READ);
	close(pipe_fd[READ]);
	close(base_fd[READ]);
	if (ast->right->exit_status == DEFAULT_EXIT_STATUS)
		ast->pid = ast->right->pid;
	else
		ast->exit_status = ast->right->exit_status;
}

// TODO: does every sub process command update the last exit?
void	create_sub_shell(t_env sub_env, char *input, t_ast *ast)
{
	t_ast	*sub_ast;
	int		sub_stdio[2];

	ast->pid = fork();
	if (ast->pid)
		return ;
	errno = 0;
	sub_ast = parser(input);
	if (!input)//TODO syntax error should be found before i think?
	{
		exit(1);
	}
	// dup2(ast->pipe[WRITE], WRITE);
	sub_stdio[READ] = dup(READ);
	sub_stdio[WRITE] = dup(WRITE);
	reset_stdio(RESET_STDIO_CLEAN);
	dup2(sub_stdio[READ], READ);
	dup2(sub_stdio[WRITE], WRITE);
	reset_stdio(RESET_STDIO_INIT);
	if (errno)
	{
		print_error(true, NULL, NULL, strerror(errno));
		exit(errno);
	}
	// TODO: differ between syntax and malloc parser error
	if (!sub_ast)
		exit(1);
	add_global_data(sub_ast, &sub_env, ast->envs); // TODO envs
	sub_ast->exit_status = ast->exit_status;
	sub_ast->env = &sub_env;
	run_node(sub_ast);
	if (sub_ast->exit_status == DEFAULT_EXIT_STATUS)
	{
		waitpid(sub_ast->pid, &(sub_ast->exit_status), 0);
		sub_ast->exit_status = WEXITSTATUS(sub_ast->exit_status);
	}
	wait_all_children();
	ast->exit_status = sub_ast->exit_status;
	free_ast(sub_ast);
	exit(ast->exit_status);
	return ;
}

// potential issue?: fds from subshell node copied to subshell fds, if they get overwritten might be bad idk
void	run_subshell(t_ast *ast)
{
	t_env	sub_env;

	if (ast->env->stop_execution && ast->exit_status == DEFAULT_EXIT_STATUS)
		ast->exit_status = 1;
	if (ast->exit_status != DEFAULT_EXIT_STATUS)
		return ;
	ft_memcpy(&sub_env, ast->env, sizeof(t_env));
	sub_env.main_process = false;
	create_sub_shell(sub_env, ast->name->token->str_data, ast);
	if (ast->pid == -1)
	{
		ast->exit_status = errno;
		print_error(true, "Subshell", NULL, strerror(errno));
	}
}

// kinda workarround for wrong build ast for multiple condtions without subshell
void	ft_or(t_ast *ast)
{
	bool	success_left;

	if (ast->exit_status != DEFAULT_EXIT_STATUS)
		ast->left->exit_status = ast->exit_status;
	else
	{
		run_node(ast->left);
		if (ast->left->exit_status == DEFAULT_EXIT_STATUS)
		{
			//printf("or waiting pid\n");
			waitpid(ast->left->pid, &(ast->left->exit_status), 0);
			ast->left->exit_status = WEXITSTATUS(ast->left->exit_status);
		}
	}
	set_last_exit(ast->left->exit_status);//TODO mb out this behind a condtion so it only updates for process nodes not operators
	//printf("OR: left exit: %d (cur type: %s)\n", ast->left->exit_status, type_to_str_type(ast->type));
	success_left = !((bool)ast->left->exit_status);
	if (success_left)//if left had no error
		ast->right->exit_status = 0;
	run_node(ast->right);
	if (ast->right->exit_status == DEFAULT_EXIT_STATUS)
	{
		waitpid(ast->right->pid, &(ast->right->exit_status), 0);
		ast->right->exit_status = WEXITSTATUS(ast->right->exit_status);
	}
	set_last_exit(ast->right->exit_status);//TODO mb out this behind a condtion so it only updates for process nodes not operators
	ast->exit_status = ast->right->exit_status;
	//printf("OR: logical right exit: %d (cur type: %s)\n", ast->right->exit_status, type_to_str_type(ast->type));
	//printf("OR: RESULT: %d\n", ast->exit_status);
}

// kinda workarround for wrong build ast for multiple condtions without subshell
void	ft_and(t_ast *ast)
{
	bool	success_left;

	if (ast->exit_status != DEFAULT_EXIT_STATUS)
		ast->left->exit_status = ast->exit_status;
	else
	{
		run_node(ast->left);
		if (ast->left->exit_status == DEFAULT_EXIT_STATUS)
		{
			waitpid(ast->left->pid, &(ast->left->exit_status), 0);
			ast->left->exit_status = WEXITSTATUS(ast->left->exit_status);
		}
	}
	set_last_exit(ast->left->exit_status);//TODO mb out this behind a condtion so it only updates for process nodes not operators
	//printf("AND: logical left exit: %d (cur type: %s)\n", ast->left->exit_status, type_to_str_type(ast->type));
	success_left = !((bool)ast->left->exit_status);
	if (!success_left)//if left had an error
		ast->right->exit_status = ast->left->exit_status;
	run_node(ast->right);
	if (ast->right->exit_status == DEFAULT_EXIT_STATUS)
	{
		waitpid(ast->right->pid, &(ast->right->exit_status), 0);
		ast->right->exit_status = WEXITSTATUS(ast->right->exit_status);
	}
	set_last_exit(ast->right->exit_status);//TODO mb out this behind a condtion so it only updates for process nodes not operators
	ast->exit_status = ast->right->exit_status;
	//printf("AND: logical right exit: %d (cur type: %s)\n", ast->right->exit_status, type_to_str_type(ast->type));
	//printf("AND: RESULT : %d\n", ast->exit_status);
}

void	init_command(t_ast *ast)
{
	if (ast->env->stop_execution)
	{
		ast->exit_status = 1;
		return ;
	}
	if (!expansion(ast))
	{
		printf("DEBUG create_sub\n");
		exit(1);
		return ;
	}
	if (resolve_redirs(ast) == ERROR)
		return ;
	if (ast->exit_status != DEFAULT_EXIT_STATUS)
		return ;
	redir_fds();
	run_command_node(ast);
	cleanup_fds();
	set_last_exit(ast->exit_status);
}

void	run_node(t_ast *ast)
{
	if (!ast)
		return ;
	if (ast->type == AND)
		ft_and(ast);
	else if (ast->type == OR)
		ft_or(ast);
	if (ast->exit_status == DEFAULT_EXIT_STATUS)
	{
		if (ast->type == PIPE)
			ft_pipe(ast);
		if (ast->name && ast->name->token->type == SUBSHELL)
			run_subshell(ast);
		else if (ast->type == COMMAND)
			init_command(ast);
	}
}
