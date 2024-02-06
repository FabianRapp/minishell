/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_ast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 12:08:53 by frapp             #+#    #+#             */
/*   Updated: 2024/02/06 15:47:19 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	create_sub(t_ast *ast);

bool	ft_pipe(t_ast *ast)
{
	int	pipe_fd[2];

	if (ast->fd[IN] != ast->base_fd[IN])
		ast->left->fd[IN] = ast->fd[IN];
	if (pipe(pipe_fd) == -1)
	{//handle error
	}
	ast->left->fd[OUT] = pipe_fd[OUT];
	ast->right->fd[IN] = pipe_fd[IN];
	run_node(ast->left);
	run_node(ast->right);
	ast->exit_fd[IN] = ast->right->exit_fd[IN];
	ast->exit_fd[OUT] = ast->right->exit_fd[OUT];
	if (ast->right->type != COMMAND && ast->right->type != SUBSHELL && ast->right->type != PIPE)
		ast->exit_status_node = ast->right->exit_status_node;
	ast->right->exit_status_node >>= 8;
	ast->exit_status_node = ast->right->exit_status_node;
	return (true);
}

pid_t	create_sub_shell(t_env sub_env, char *input, t_ast *ast)
{
	t_ast	*sub_ast;
	pid_t	pid;
	int		temp;

	//
	pid = fork();
	sub_ast =  parser(input);
	if (pid)
	{
		//reset_stdio(ast);
		return (pid);
	}
	redir_stdio(ast);
	if (!input)
	{
		my_exit(ast, 1);
	}
	// TODO: differ between syntax and malloc parser error
	if (!sub_ast)
		my_exit(ast, 1);
	//if (ast->fd[IN] != ast->base_fd[IN] || ast->fd[OUT] != ast->base_fd[OUT])
		
	add_global_data(sub_ast, &sub_env, ast->envs); // TODO envs
	
	sub_ast->exit_status_node = ast->exit_status_node;
	sub_ast->base_fd[0] = ast->base_fd[0];
	sub_ast->base_fd[1] = ast->base_fd[1];
	sub_ast->env = &sub_env;
	run_node(sub_ast);
	if (sub_ast->type == AND || sub_ast->type == OR)
		sub_env.exit_status = sub_ast->exit_status_node;
	else
	{
		read(sub_ast->exit_fd[IN], &(sub_env.exit_status), sizeof(int));
	}
	while (waitpid(-1, &temp, 0) != -1)
	{
		// sub_env.exit_status = temp;
	}
	temp = sub_env.exit_status;
	ast->exit_status_node = sub_ast->exit_status_node;
	free_ast(sub_ast);
	free_env(&sub_env);
	my_exit(ast, temp);
	return (-1);
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
	pid = create_sub_shell(sub_env, ast->name->token->str_data, ast);
	if (pid != 0 && pid != -1) //parent
	{
		//close(ast->exit_fd[OUT]);
		
		
		//printf("pid: %d\n", pid);
		//waitpid(pid, &(ast->exit_status_node), 0);
		//ast->env->exit_status = ast->exit_status_node;
		//if (read(ast->exit_fd[IN], &(ast->exit_status_node), sizeof(int)) == -1)
		{//handle error
			//ft_fprintf(ast->base_fd[OUT], "DEBUG: read fauled run_subshell\n");
		}
		//ast->exit_status_node >>= 8;
		//printf("debug: subshell exit status: %d (str: %s)\n", ast->exit_status_node, ast->name->token->str_data);
	}
	// else
	// {
	// 	ast->exit_status_node = 1;
	// 	ast->env->exit_status = 1;
	// }
}


void	condtion_check(t_ast *ast)
{
if (ast->right != NULL) {
		printf("ast->right is not NULL, type: %s\n", type_to_str_type(ast->right->type));

		// Check the first part of the OR condition
		if (ast->right->type == COMMAND) {
			printf("ast->right->type is COMMAND\n");

			// Since ast->right->name or ast->right->name->token could be NULL, check them to avoid segmentation faults
			if (ast->right->name != NULL && ast->right->name->token != NULL) {
				printf("ast->right->name and ast->right->name->token are not NULL\n");

				if (ast->right->name->token->type == COMMAND) {
					printf("ast->right->name->token->type is COMMAND\n");
				} else {
					printf("ast->right->name->token->type is NOT COMMAND, type: %s\n", type_to_str_type(ast->right->name->token->type));
				}
			} else {
				if (ast->right->name == NULL) {
					printf("ast->right->name is NULL\n");
				}
				if (ast->right->name->token == NULL) {
					printf("ast->right->name->token is NULL\n");
				}
			}
		} else {
			printf("ast->right->type is NOT COMMAND, type: %s\n", type_to_str_type(ast->right->type));
		}

		// Check the second part of the OR condition
		if (ast->right->type == SUBSHELL) {
			printf("ast->right->type is SUBSHELL\n");
		} else {
			printf("ast->right->type is NOT SUBSHELL, type: %s\n", type_to_str_type(ast->right->type));
		}

		// Check the AND condition with exit_status_node
		if (ast->right->exit_status_node == DEFAULT_EXIT_STATUS) {
			printf("ast->right->exit_status_node is DEFAULT_EXIT_STATUS\n");
		} else {
			printf("ast->right->exit_status_node is NOT DEFAULT_EXIT_STATUS\n");
		}
	} else {
		printf("ast->right is NULL\n");
	}
	
	// Now, combining the conditions with logical operators to see if the entire condition is true
	if (ast->right != NULL &&
		(((ast->right->type == COMMAND &&
		ast->right->name != NULL &&
		ast->right->name->token != NULL &&
		ast->right->name->token->type == COMMAND) ||
		ast->right->type == SUBSHELL) &&
		ast->right->exit_status_node == DEFAULT_EXIT_STATUS)) {
		printf("Entire condition is TRUE\n");
	} else {
		printf("Entire condition is FALSE\n");
	}
}
// kinda workarround for wrong build ast for multiple condtions without subshell
void	ft_or(t_ast *ast)
{
	if (ast->exit_status_node == DEFAULT_EXIT_STATUS)
	{
		run_node(ast->left);
		
		// ast->exit_fd[IN] = ast->left->exit_fd[IN];
		//if (ast->left->type != AND && ast->left->type != OR)
		if (ast->left->type == COMMAND || ast->left->type == PIPE)//|| ast->left->type == SUBSHELL
		{
			read(ast->left->exit_fd[IN], &(ast->left->exit_status_node), sizeof(int)); //TODO read fail
			//ast->left->exit_status_node >>= 8;
			close(ast->left->exit_fd[IN]);
		}
		else
		{
			printf("OR: else ft_or wtf\n");
		}
	}
	else
	{
		ast->left->exit_status_node = ast->exit_status_node;
		//printf("OR: left copied from operator node in %s: %d\n", type_to_str_type(ast->type), ast->exit_status_node);
	}
	//printf("OR: logical left exit: %d (cur type: %s)\n", ast->left->exit_status_node, type_to_str_type(ast->type));
	if (!((bool)ast->left->exit_status_node))//left no error
	{
		ast->exit_status_node = ast->left->exit_status_node;
		ast->right->exit_status_node = 0;
	}
	else
	{
		//ast->right->exit_status_node = ast->exit_status_node;
	}
	run_node(ast->right);
	ast->exit_fd[IN] = ast->right->exit_fd[IN];
	if ((ast->right->type == COMMAND || ast->right->type == SUBSHELL || ast->right->type == PIPE) && ast->left->exit_status_node) // TODO coisnider dummys?
	{
		//printf("OR: reading with %s\n", type_to_str_type(ast->right->type));
		read(ast->right->exit_fd[IN], &(ast->right->exit_status_node), sizeof(int)); //TODO read fail
		close(ast->right->exit_fd[IN]);
	}
	//else
		ast->exit_status_node = ast->right->exit_status_node;
	//printf("OR: logical right exit: %d (cur type: %s)\n", ast->right->exit_status_node, type_to_str_type(ast->type));
	//printf("OR: RESULT: %d\n", ast->exit_status_node);
}


// kinda workarround for wrong build ast for multiple condtions without subshell
void	ft_and(t_ast *ast)
{
	if (ast->exit_status_node == DEFAULT_EXIT_STATUS)
	{
		run_node(ast->left);
		// ast->exit_fd[IN] = ast->left->exit_fd[IN];
		//if (ast->left->type != AND && ast->left->type != OR)
		if (ast->left->type == COMMAND || ast->left->type == PIPE)// || ast->left->type == SUBSHELL 
		{
			read(ast->left->exit_fd[IN], &(ast->left->exit_status_node), sizeof(int)); //TODO read fail
			close(ast->left->exit_fd[IN]);
		}
		else
		{
			printf("AND: else ft_and wtf\n");
		}
	}
	else
	{
		ast->left->exit_status_node = ast->exit_status_node;
		//printf("AND: left copied from operator node in %s: %d\n", type_to_str_type(ast->type), ast->exit_status_node);
	}
	//printf("AND: logical left exit: %d (cur type: %s)\n", ast->left->exit_status_node, type_to_str_type(ast->type));
	if (((bool)ast->left->exit_status_node))
	{
		ast->exit_status_node = ast->left->exit_status_node;
		ast->right->exit_status_node = ast->left->exit_status_node;
		
	}
	run_node(ast->right);
	ast->exit_fd[IN] = ast->right->exit_fd[IN];
	
	if ((ast->right->type == COMMAND || ast->right->type == SUBSHELL || ast->right->type == PIPE) && !ast->left->exit_status_node) // TODO coisnider dummys?
	{
		//printf("reading with %s\n", type_to_str_type(ast->right->type));
		read(ast->right->exit_fd[IN], &(ast->right->exit_status_node), sizeof(int)); //TODO read fail
		close(ast->right->exit_fd[IN]);
	}
	ast->exit_status_node = ast->right->exit_status_node;
	//printf("AND: logical right exit: %d (cur type: %s)\n", ast->right->exit_status_node, type_to_str_type(ast->type));
	//printf("AND: RESULT : %d\n", ast->exit_status_node);
}



void	run_node(t_ast *ast)
{
	if (!ast)
		return ;
	//print_ast(ast);
	if (ast->type == AND)
		ft_and(ast);
	else if (ast->type == OR)
		ft_or(ast);
	else if (ast->type == PIPE)
		ft_pipe(ast);
	else if (ast->exit_status_node == DEFAULT_EXIT_STATUS)
	{
		if (ast->name && ast->name->token->type == SUBSHELL)
			run_subshell(ast);
		else if (ast->type == COMMAND)
			create_sub(ast);
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
		printf("error creating pipe\n");
		exit(errno);
	}
	if (!expand_strs(ast))
	{
		printf("DEBUG create_sub\n");
		exit(1);
		return ;
	}
	resolve_redirs(ast);
	if (check_edgecases(ast))
	{
		return ;
	}
	ast->id = fork();
	if (ast->id == -1)
	{
		ast->exit_status_node = errno;
		printf("error creating fork\n");
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
