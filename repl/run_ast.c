/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_ast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 12:08:53 by frapp             #+#    #+#             */
/*   Updated: 2024/02/03 22:16:05 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	create_sub(t_ast *ast);

bool	ft_pipe(t_ast *ast)
{
	// if (pipe(ast->fd) == -1)
	// {//handle error
	// }
	// if (dup2(, STDOUT_FILENO) == -1)
	// {//handle error
	// }
	// if (dup2(, STDIN_FILENO) == -1)
	// {//handle error
	// }
	run_node(ast->left);
	run_node(ast->right);
	ast->exit_fd[IN] = ast->right->exit_fd[IN];
	return (true);
}

pid_t	create_sub_shell(t_env sub_env, char *input, int exit_fd[2], t_ast *ast)
{
	t_ast	*sub_ast;
	pid_t	pid;
	int		temp;

	pid = fork();
	if (pid)
		return (pid);
	if (!input)
	{
		exit(1);
	}
	sub_ast =  parser(input);
	// TODO: differ between syntax and malloc parser error
	if (!sub_ast)
		exit(1);
	exit_fd[IN] = sub_ast->exit_fd[IN] ;
	exit_fd[OUT] = sub_ast->exit_fd[OUT];
	sub_ast->base_fd[0] = ast->base_fd[0];
	sub_ast->base_fd[1] = ast->base_fd[1];
	sub_ast->env = &sub_env;
	run_node(sub_ast);
	temp = sub_env.exit_status;
	free_ast(sub_ast);
	free_env(&sub_env);
	exit(temp);
}

// potential issue?: fds from subshell node copied to subshell fds, if they get overwritten might be bad idk
void	run_subshell(t_ast *ast)
{
	t_env	sub_env;
	pid_t	pid;

	memcpy(&sub_env, ast->env, sizeof(t_env));
	sub_env.main_process = false;
	if (pipe(ast->exit_fd) == -1)
	{//handle error
	}
	pid = create_sub_shell(sub_env, ast->name->token->str_data, ast->exit_fd, ast);
	if (pid != 0 && pid != -1) //parent
	{
		close (ast->exit_fd[OUT]);
		//waitpid(pid, &(ast->exit_status_node), 0);
		//ast->env->exit_status = ast->exit_status_node;
	}
	else
	{
		ast->exit_status_node = 1;
		ast->env->exit_status = 1;
	}
}

void	logical(t_ast *ast, bool left_exit_right_conditon)
{
	run_node(ast->left);
	if (ast->right->type != AND && ast->right->type != OR)
	{
		read(ast->left->exit_fd[IN], &(ast->left->exit_status_node), sizeof(int)); //TODO read fail
		close(ast->left->exit_fd[IN]);
	}
	if (((bool)ast->left->exit_status_node) != left_exit_right_conditon)
		ast->exit_status_node = ast->left->exit_status_node;
	else
	{
		run_node(ast->right);
		if (ast->right->type == COMMAND && ast->right->name->token->type == COMMAND)
		{
			read(ast->right->exit_fd[IN], &(ast->right->exit_status_node), sizeof(int)); //TODO read fail
			close(ast->right->exit_fd[IN]);
		}
		ast->exit_status_node = ast->right->exit_status_node;
	}
}

void	run_node(t_ast *ast)
{
	if (!ast)
		return ;
	if (ast->type == AND)
		logical(ast, false);
	else if (ast->type == OR)
		logical(ast, true);
	else if (ast->type == PIPE)
		ft_pipe(ast);
	else if (ast->name && ast->name->token->type == SUBSHELL)
		run_subshell(ast);
	else if (ast->type == COMMAND)
	{
		create_sub(ast);
		//my_exit(ast, ast->exit_status_node);
	}
}

//TODO need a mock_my_exit function for endecases, redirs and ft fucntions so other nodes dont read forever
void	create_sub(t_ast *ast)
{
	// if (ast->type == COMMAND)
	// 	run_node

	if (pipe(ast->exit_fd) == -1)
	{	
		ast->exit_status_node = errno;
		exit(errno);
	}
	if (!expand_strs(ast))
		return ;
	resolve_redirs(ast);
	if (check_edgecases(ast))
	{
		return ;
	}
	ast->id = fork();
	if (ast->id == -1)
	{
		ast->exit_status_node = errno;
		exit(errno);
	}
	
	if (ast->id != 0)
		return ;
	//if (ast->type == COMMAND)
		run_command_node(ast);
}

// expects tree syntax to be ok on the token level
void	walk_ast(t_ast *ast)
{
	if (!ast)
		return ;
	create_sub(ast);
	//walk_ast(ast->left);
	//walk_ast(ast->right);
}
