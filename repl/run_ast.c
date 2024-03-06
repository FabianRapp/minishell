/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_ast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 12:08:53 by frapp             #+#    #+#             */
/*   Updated: 2024/03/06 07:11:17 by frapp            ###   ########.fr       */
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
	//int		sub_stdio[2];

	sub_ast = parser(input);
	if (!sub_ast)
	{
		ast->exit_status = get_last_exit();
		return ;
	}
	ast->pid = fork();
	errno = 0;
	if (ast->pid)
	{
		//check_fds();
		free_ast(sub_ast);
		return ;
	}
	
	// sub_stdio[READ] = dup(READ);
	// sub_stdio[WRITE] = dup(WRITE);
	// dup2(sub_stdio[READ], READ);
	// dup2(sub_stdio[WRITE], WRITE);
	if (resolve_redirs(ast) == ERROR)
	{
		cleanup_fds();
		return ;
	}
	redir_fds();
	io_data(RESET_FDS, NULL);
	
	// if (sub_stdio[READ] == -1 || sub_stdio[WRITE] == -1
	// 	|| dup2(sub_stdio[READ], READ) == -1
	// 	|| dup2(sub_stdio[WRITE], WRITE) == -1)
	if (errno)
	{
		print_error(true, NULL, NULL, strerror(errno));
		exit(errno);
	}
	add_global_data(sub_ast, &sub_env, ast->envs);
	sub_ast->exit_status = ast->exit_status;
	sub_ast->env = &sub_env;
	run_node(sub_ast);
	//ft_fprintf(2, "test\n");
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
	char	*input;

	if (ast->env->stop_execution && ast->exit_status == DEFAULT_EXIT_STATUS)
		ast->exit_status = 1;
	if (ast->exit_status != DEFAULT_EXIT_STATUS)
		return ;
	ft_memcpy(&sub_env, ast->env, sizeof(t_env));
	input = ast->name->token->str_data;
	if (!input)
	{
		print_error(true, "DEBUG leave this error check in but it should never show (create_sub_shell)", NULL, "Error");
		ast->exit_status = 2;
		set_last_exit(2);
		return ;
	}
	if (!*input)
	{
		print_error(true, NULL, NULL, "syntax error near unexpected token `)'");
		ast->exit_status = 2;
		set_last_exit(2);
		exit(2);
		return ;
	}

	sub_shell_mode(SET_SUB_SHELL);
	create_sub_shell(sub_env, input, ast);
	sub_shell_mode(UNSET_SUB_SHELL);
	// cleanup_fds();
	if (ast->pid == -1)
	{
		ast->exit_status = errno;
		print_error(true, NULL, NULL, strerror(errno));
	}
}

// kinda workarround for wrong build ast for multiple condtions without subshell
void	ft_or(t_ast *ast)
{
	bool	success_left;

	if (ast->exit_status != DEFAULT_EXIT_STATUS)
	{
		//printf("or gives this exit to left node: %d\n", ast->exit_status);
		ast->left->exit_status = ast->exit_status;
	}
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
		// if (ast->left->exit_status > 0)
		// {
		// 	ast->exit_status = ast->left->exit_status;
		// 	ast->right->exit_status = ast->left->exit_status;
		// }
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
	{
		cleanup_fds();
		return ;
	}
	
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
		{
			init_command(ast);
		}
	}
}
