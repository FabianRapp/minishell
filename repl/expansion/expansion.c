/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 03:37:23 by frapp             #+#    #+#             */
/*   Updated: 2024/02/09 22:20:31 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

// merges the current node with the next nodes until one of them is not
// a LITERAL node
bool	merge_literals(t_token_list *node)
{
	t_token_list	*to_free;

	while (node && node->token->type == LITERAL && node->next && node->next->token->type == LITERAL)
	{
		if (!ft_strjoin_inplace(&(node->token->str_data), node->next->token->str_data))
		{//handle malloc fail
			return (false);
		}
		to_free = node->next;
		node->next = node->next->next;
		free_token(to_free->token);
		free(to_free);
	}
	return (true);
}

t_token_list	*expand_list(t_env *env, t_token_list *list)
{
	if (list->token->type == INTERPRETED)
	{
		if (!expand_interpreted(list->token, env))
		{//malloc fail
		}
	}
	if (list->token->type == ENV_VAR)
	{
		if (!env_to_word_token(list->token))
		{// malloc fail
		}
		//expand_list(ast, list);
	}
	else if (list->token->type == PID_REQUEST)
	{
		if (!pidreq_to_literal_token(env, list->token))
		{//malloc fails
		}
	}
	else if (list->token->type == EXIT_STATUS_REQUEST)
	{
		list->token->type = LITERAL;
		list->token->str_data = get_last_exit();
		if (!list->token->str_data)
		{//malloc fails
		}
	}
	if (list->token->type == WORD)
	{
		// TODO: differ between malloc fail in word_splitting and empty list
		list = word_splitting(list);
		if (!list)
			return (NULL);
	}
	if (list->next && list->next->token->type != T_EOF)
		list->next = expand_list(env, list->next);
	if (!merge_literals(list))
	{// malloc fail
	}
	return (list);
}

bool	expand_name(t_ast *ast)
{
	if (!ast->name)
		return (true);
	ast->name = expand_list(ast->env, ast->name);// needs malloc protection
	if (ast->name)
		ast->name = remove_non_literals(ast->name);
	if (!ast->name)
	{
		ast->name = ft_calloc(1, sizeof(t_token_list));
		if (!ast->name)
		{
			ast->exit_status = errno;
			return (false);
		}
		ast->name->token = new_dummy_token();
		if (!ast->name->token)
		{
			ast->exit_status = errno;
			return (false);
		}
	}
	if (!move_excess_name_to_arg(ast))
		return (false);
	return (true);
}

bool	expand_args(t_ast *ast)
{
	if (!ast->arg)
		return (true);
	return (true);
}

bool	expand_redirs(t_ast *ast)
{
	if (!ast->redir)
		return (true);
	return (true);
}

// TODO:
// expands the current ast nodes env vars and interpreted strs (if the first)
bool	expansion(t_ast *ast)
{
	//t_token_list	*temp;
	if (!ast)
		return (false);
	if (ast->type != COMMAND)
		return (true);
	if (!expand_name(ast))
		return (false);
	//if (ast->type == COMMAND && ast->name->token->type == SUBSHELL)
		//ast->type = SUBSHELL;
	if (!expand_args(ast))
		return (false);
	if (!expand_redirs(ast))
		return (false);
	return (true);
}
