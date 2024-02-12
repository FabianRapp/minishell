/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_ast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 12:08:53 by frapp             #+#    #+#             */
/*   Updated: 2024/02/11 23:47:09 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

bool	ft_pipe(t_ast *ast)
{
	int		pipe_fd[2];
	//pid_t	pid;

	if (ast->exit_status > 0)
		return (false);
	//if (ast->fd[IN] != ast->base_fd[IN])
		ast->left->fd[IN] = ast->fd[IN];
	if (pipe(pipe_fd) == -1)
	{//handle error
	}
	if (ast->left->fd[OUT] != OUT)
		close(ast->left->fd[OUT]);
	ast->left->fd[OUT] = pipe_fd[OUT];
	if (ast->right->fd[IN] != IN)
		close(ast->right->fd[IN]);
	ast->right->fd[IN] = pipe_fd[IN];
	run_node(ast->left);
	if (ast->left->fd[OUT] != OUT)
		close(ast->left->fd[OUT]);
	run_node(ast->right);
	if (ast->right->fd[IN] != IN)
		close(ast->right->fd[IN]);
	//close(pipe_fd[OUT]);
	if (ast->right->exit_status == DEFAULT_EXIT_STATUS)
	{
		//printf("pipe right had no exit status (right type: %s)\n", type_to_str_type(ast->right->type));
		ast->pid = ast->right->pid;
	}
	else
	{
		//printf("pipe right had exit status (right type: %s)\n", type_to_str_type(ast->right->type));
		ast->exit_status = ast->right->exit_status;
	}
	return (true);
}

// TODO: does every sub process command update the last exit?
void	create_sub_shell(t_env sub_env, char *input, t_ast *ast)
{
	t_ast	*sub_ast;
	int		temp;

	ast->pid = fork();
	if (ast->pid)
		return ;
	redir_stdio(ast);
	sub_ast = parser(input);
	if (!input)//TODO syntax error should be found before i think?
	{
		exit(1);
	}
	// TODO: differ between syntax and malloc parser error
	if (!sub_ast)
		exit(1);
	//if (ast->fd[IN] != ast->base_fd[IN] || ast->fd[OUT] != ast->base_fd[OUT])
	add_global_data(sub_ast, &sub_env, ast->envs); // TODO envs
	sub_ast->exit_status = ast->exit_status;
	sub_ast->base_fd[0] = ast->fd[0];//
	sub_ast->base_fd[1] = ast->fd[1];//
	sub_ast->env = &sub_env;
	run_node(sub_ast);
	if (sub_ast->exit_status == DEFAULT_EXIT_STATUS)
	{
		waitpid(sub_ast->pid, &(sub_ast->exit_status), 0);
		sub_ast->exit_status = WEXITSTATUS(sub_ast->exit_status);
	}
	while (waitpid(-1, &temp, 0) != -1)
	{
	}
	ast->exit_status = sub_ast->exit_status;
	free_ast(sub_ast);
	free_env(&sub_env);
	exit(ast->exit_status);
	return ;
}

// potential issue?: fds from subshell node copied to subshell fds, if they get overwritten might be bad idk
void	run_subshell(t_ast *ast)
{
	t_env	sub_env;

	if (ast->exit_status != DEFAULT_EXIT_STATUS)
		return ;
	memcpy(&sub_env, ast->env, sizeof(t_env));
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
	if (!expansion(ast))
	{
		printf("DEBUG create_sub\n");
		exit(1);
		return ;
	}
	resolve_redirs(ast);
	if (ast->exit_status != DEFAULT_EXIT_STATUS)
		return ;
	run_command_node(ast);
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

