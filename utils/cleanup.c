/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 06:07:22 by frapp             #+#    #+#             */
/*   Updated: 2024/03/26 22:37:10 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	free_token_list(t_token_list *list)
{
	t_token_list	*last;

	while (list)
	{
		last = list;
		list = list->next;
		free_token(last->token);
		free(last);
	}
}

void	free_arg_list(t_arg *list)
{
	t_arg	*last;

	while (list)
	{
		last = list;
		list = list->next;
		free_token_list(last->name);
		free(last);
	}
}

/*
	Iterates through and frees all associated redirs and argument structures.
	If the here-doc/-str arg starts with "<<<<" that means
	the following integer string corresponds to an open fd
*/
static void	free_redir(t_redir *redir)
{
	t_redir	*last;

	while (redir)
	{
		if (redir->type == HERE_DOC || redir->type == HERE_STR)
		{
			if (redir->token_str_data
				&& !ft_strncmp(redir->token_str_data,
					"<<<<", 4))
			{
				close(ft_atoi(redir->token_str_data + 4));
				ft_free((void **)&(redir->token_str_data));
			}
		}
		free_arg_list(redir->arg);
		last = redir;
		redir = redir->next;
		free(last);
	}
}

void	free_ast(t_ast *ast)
{
	if (ast->left)
		free_ast(ast->left);
	if (ast->right)
		free_ast(ast->right);
	free_token_list(ast->name);
	free_arg_list(ast->arg);
	free_redir(ast->redir);
	free(ast);
}

t_result	wait_all_children(t_ast *ast)
{
	int		status;

	errno = 0;
	if (ast && ast->pid != INIT_VAL && ast->exit_status == INIT_VAL)
		waitpid(ast->pid, &status, 0);
	while (errno != ECHILD)
	{
		waitpid(-1, &status, 0);
	}
	errno = 0;
	return (SUCCESS);
}
