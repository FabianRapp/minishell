/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:38:43 by frapp             #+#    #+#             */
/*   Updated: 2024/01/29 02:38:58 by frapp            ###   ########.fr       */
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