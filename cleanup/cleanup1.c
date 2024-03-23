/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 06:07:22 by frapp             #+#    #+#             */
/*   Updated: 2024/03/23 02:45:15 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"
#include "../headers/lexer.h"

/*
	Iterates through and frees all associated redirection and argument structures.
	If the here-doc arg starts with "<<<<" that means the following integer string
	corresponds to an open fd
*/
void	free_redir(t_redir *redir)
{
	t_redir	*last;

	while (redir)
	{
		if (redir->type == HERE_DOC)
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
