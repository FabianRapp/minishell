/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:38:43 by frapp             #+#    #+#             */
/*   Updated: 2024/01/31 13:50:33 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

int	count_args(t_ast *ast, int type)
{
	t_arg	*args;
	int		count;

	if (type == ARGS)
		args = ast->arg;
	else if (type == OUT)
		args = ast->redir_out;
	else if (type == IN)
		args = ast->redir_in;
	else
		return (0);
	count = 0;
	while (args && args->type != T_EOF)
	{
		count++;
		args = args->next;
	}
	return (count);
}

// do not free the strings in argv, they are wihtin the tokens and are hadnled by cleanup
void	fill_args(t_ast *ast, char *argv[], int type)
{
	t_arg	*args;
	int		i;

	if (type == ARGS)
		args = ast->arg;
	else if (type == REDIR_OUT)
		args = ast->redir_out;
	else if (type == REDIR_IN)
		args = ast->redir_in;
	else
		return ;
	i = 0;
	while (args && args->type != T_EOF)
	{
		argv[i++] = args->name->token->str_data; // assumes name to be fully expanded (not the case right now)
		args = args->next;
	}
	argv[i] = NULL;
}

int	get_pid(void)
{
	pid_t	pid;
	int		exit_status;
	int		fd[2];

	if (pipe(fd) == -1)
		return(print_error(true, false, false, "Error creating pipe\n"), 0);
	pid = fork();
	if (pid != 0 && pid != -1) // main
	{
		close(fd[0]);
		write(fd[1], &pid, sizeof(pid));
		close(fd[1]);
		waitpid(pid, &exit_status, 0);
		return (exit(exit_status), 0);
	}
	else if (pid == 0)
	{
		close(fd[1]);
		read(fd[0], &pid, sizeof(pid_t));
		close(fd[0]);
		return(pid);
	}
	return (print_error(true, NULL, NULL, "error forking main process"), 0);
}
