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

// for here doc: dollar sign is not interpreted
bool	add_dollar(t_token *token)
{
	if (token->type == ENV_VAR)
	{
		free(token->str_data);
		token->str_data = ft_strjoin("$", token->old_data);
	}
	else if (token->type == PID_REQUEST)
		token->str_data = ft_strjoin("$", "$");
	else if (token->type == EXIT_STATUS_REQUEST)
		token->str_data = ft_strjoin("$", "?");
	token->type = LITERAL;
	if (!token->str_data)
		return (false);
	return (true);
}

t_token_list	*expand_list(t_env *env, t_token_list *list, bool expand_dollar)
{
	if (list->token->type == INTERPRETED)
	{
		if (expand_dollar)
		{
			if (!expand_interpreted(list->token, env))
			{//malloc fail
			}
		}
		list->token->type = LITERAL;
	}
	if (list->token->type == ENV_VAR)
	{
		if (expand_dollar)
		{
			if (!env_to_word_token(list->token))
			{// malloc fail
			}
		}
		else
		{
			if (!add_dollar(list->token))
			{//malloc fail
			}
		}
	}
	else if (list->token->type == PID_REQUEST)
	{
		if (expand_dollar)
		{
			if (!pidreq_to_literal_token(env, list->token))
			{//malloc fails
			}
		}
		else
		{
			if (!add_dollar(list->token))
			{//malloc fail
			}
		}
	}
	else if (list->token->type == EXIT_STATUS_REQUEST)
	{
		if (expand_dollar)
		{
			list->token->str_data = get_last_exit_str();
			if (!list->token->str_data)
			{//malloc fails
			}
			list->token->type = LITERAL;
		}
		else
		{
			if (!add_dollar(list->token))
			{//malloc fail
			}
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
		list->next = expand_list(env, list->next, expand_dollar);
	if (!merge_literals(list))
	{// malloc fail
	}
	return (list);
}


bool	expand_name(t_ast *ast)
{
	if (!ast->name)
		return (true);
	ast->name = expand_list(ast->env, ast->name, true);// needs malloc protection
	if (ast->name)
		ast->name = remove_non_literals(ast->name);
	if (errno)
		return (ERROR);
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
		return (true);
	}
	if (!move_excess_name_to_arg(ast))
		return (false);
	return (true);
}

bool	expand_args(t_ast *ast, t_arg **base_arg, bool expand_dollar)
{
	t_arg	*cur;
	t_arg	*new_arg;
	t_arg	*last;

	cur = *base_arg;
	if (!cur)
		return (true);
	last = NULL;
	while (cur)
	{
		cur->name = expand_list(ast->env, cur->name, expand_dollar);
		if (cur->name)
			cur->name = remove_non_literals(cur->name);
		if (errno)
			return (ERROR);
		if (!cur->name)//remove the current arg
		{
			if (last)
			{
				last->next = cur->next;
				free(cur);
				cur = last->next;
			}
			else
			{
				if (*base_arg)
				{
					cur = ast->arg->next;
					free(ast->arg);
					*base_arg = cur;
				}
				else
					return (true);
			}
			continue ;
		}
		while (cur->name && cur->name->next)
		{
			new_arg = ft_calloc(1, sizeof(t_arg));
			new_arg->next = cur->next;
			new_arg->name = cur->name->next;
			cur->name->next = cur->name->next->next;
			new_arg->name->next = NULL;
			cur->next = new_arg;
		}
		last = cur;
		cur = cur->next;
	}
	return (true);
}

bool	expand_redirs(t_ast *ast)
{
	t_redir	*cur;

	cur = ast->redir;
	if (!cur)
		return (true);
	while (cur)
	{
		if (cur->type != HERE_DOC)
			expand_args(ast, &(cur->arg), true);
		else
			expand_args(ast, &(cur->arg), false);
		cur = cur->next;
	}
	return (true);
}

// TODO:
t_result	expansion_iter(t_ast *ast)
{
	//t_token_list	*temp;
	if (!ast)
		return (ERROR);
	if (ast->type != COMMAND)
		return (SUCCESS);
	if (!expand_name(ast))
		return (ERROR);
	//if (ast->type == COMMAND && ast->name->token->type == SUBSHELL)
		//ast->type = SUBSHELL;
	if (!expand_args(ast, &(ast->arg), true))
		return (ERROR);
	if (!expand_redirs(ast))
		return (ERROR);
	return (SUCCESS);
}


// TODO:
// expands the current ast nodes env vars and interpreted strs (if the first)
bool	expansion(t_ast *ast)
{
	if (expansion_iter(ast) == ERROR)
		return (ERROR);
	if (expand_wildcards(ast) == ERROR)
		return (ERROR);
	if (expansion_iter(ast) == ERROR)
		return (ERROR);
	return (SUCCESS);
}
