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

// for error messages where the base string is needed and here_doc expansion
t_result	add_dollar(t_token *token)
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
		return (ERROR);
	return (SUCCESS);
}

t_token_list	*expand_list_here_doc(t_env *env, t_token_list *list)
{
	if (list->token->type == INTERPRETED)
		list->token->type = LITERAL;
	if (list->token->type == ENV_VAR
		|| list->token->type == PID_REQUEST
		|| list->token->type == EXIT_STATUS_REQUEST)
	{
		if (add_dollar(list->token) == ERROR)
			return (list);
	}
	if (list->token->type == WORD)
	{
		// TODO: differ between malloc fail in word_splitting and empty list
		list = word_splitting(list);
		if (!list)
			return (NULL);
	}
	if (list->next && list->next->token->type != T_EOF)
		list->next = expand_list_here_doc(env, list->next);
	if (errno)
		return (list);
	merge_literals(list);
	return (list);
}

t_token_list	*expand_list_normal(t_env *env, t_token_list *list)
{
	if (list->token->type == INTERPRETED)
	{
		if (expand_interpreted(list->token, env) == ERROR)
			return (list);
		list->token->type = LITERAL;
	}
	if (list->token->type == ENV_VAR)
	{
		if (env_to_word_token(list->token) == ERROR)
			return (list);
	}
	else if (list->token->type == PID_REQUEST)
	{
		if (pidreq_to_literal_token(env, list->token) == ERROR)
			return (list);
	}
	else if (list->token->type == EXIT_STATUS_REQUEST)
	{
		list->token->str_data = get_last_exit_str();
		if (!list->token->str_data)
			return (list);
		list->token->type = LITERAL;
	}
	if (list->token->type == WORD)
	{
		// TODO: differ between malloc fail in word_splitting and empty list
		list = word_splitting(list);
		if (!list)
			return (NULL);
	}
	if (list->next && list->next->token->type != T_EOF)
		list->next = expand_list_normal(env, list->next);
	if (errno)
		return (list);
	if (!merge_literals(list))
	{// malloc fail
	}
	return (list);
}

// TODO needs refactor
t_result	expansion_move_to_arg(t_arg **start_next, t_token_list *list)
{
	t_arg	*new_arg;
	t_arg	old_next;
	t_arg	*next_arg;

	next_arg = *start_next;
	while (list && list->next)
	{
		new_arg = ft_calloc(1, sizeof(t_arg));
		if (!new_arg)
			return (ERROR);
		new_arg->name = list->next;
		list->next = list->next->next;
		new_arg->name->next = NULL;
		if (!next_arg)
		{
			if (!*start_next)
				*start_next = new_arg;
			else
			{
				next_arg = *start_next;
				while (next_arg->next)
					next_arg = next_arg->next;
				next_arg->next = new_arg;
				next_arg = NULL;
			}
		}
		else
		{
			old_next = *next_arg;
			*next_arg = *new_arg;
			*new_arg = old_next;
			next_arg->next = new_arg;
			next_arg = next_arg->next;
		}
	}
	return (SUCCESS);
}

t_result	expand_name(t_ast *ast)
{
	if (!ast->name)
		return (SUCCESS);
	ast->name = expand_list_normal(ast->env, ast->name);// needs malloc protection
	if (errno)
		return (ERROR);
	if (wildcards_expand_name(ast->name) == ERROR)
		return (ERROR);
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
			return (ERROR);
		}
		ast->name->token = new_dummy_token();
		if (!ast->name->token)
		{
			ast->exit_status = errno;
			return (ERROR);
		}
		return (SUCCESS);
	}
	if (expansion_move_to_arg(&(ast->arg), ast->name->next) == ERROR)
	{
		ast->exit_status = errno;
		return (ERROR);
	}
	return (SUCCESS);
}

bool	expand_args(t_ast *ast, t_arg **base_arg, bool not_here_doc)
{
	t_arg	*cur;
	t_arg	*last;

	cur = *base_arg;
	if (!cur)
		return (true);
	last = NULL;
	while (cur)
	{
		if (not_here_doc)
			cur->name = expand_list_normal(ast->env, cur->name);
		else
			cur->name = expand_list_here_doc(ast->env, cur->name);
		if (errno)
		{
			ast->exit_status = errno;
			return (ERROR);
		}
		if (wildcards_expand_name(cur->name) == ERROR)
		{
			ast->exit_status = 1;
			return (ERROR);
		}
		if (cur->name)
			cur->name = remove_non_literals(cur->name);
		if (errno)
		{
			ast->exit_status = errno;
			return (ERROR);
		}
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
					return (SUCCESS);
			}
			continue ;
		}
		if (expansion_move_to_arg(&(cur->next), cur->name) == ERROR)
		{
			ast->exit_status = errno;
			return (ERROR);
		}
		last = cur;
		cur = cur->next;
	}
	return (SUCCESS);
}

t_result	expand_redirs(t_ast *ast)
{
	t_redir	*cur;

	cur = ast->redir;
	if (!cur)
		return (true);
	while (cur)
	{
		if (cur->type != HERE_DOC)
		{
			if (expand_args(ast, &(cur->arg), true) == ERROR)
				return (ERROR);
		}
		else
		{
			if (expand_args(ast, &(cur->arg), false) == ERROR)
				return (ERROR);
		}
		cur = cur->next;
	}
	return (SUCCESS);
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
	if (expand_args(ast, &(ast->arg), true) == ERROR)
		return (ERROR);
	if (expand_redirs(ast) == ERROR)
		return (ERROR);
	return (SUCCESS);
}

// TODO:
// expands the current ast nodes env vars and interpreted strs (if the first)
bool	expansion(t_ast *ast)
{
	if (expansion_iter(ast) == ERROR)
		return (ERROR);
	//if (expand_wildcards(ast) == ERROR)
	//	return (ERROR);
	//if (expansion_iter(ast) == ERROR)
	//	return (ERROR);
	return (SUCCESS);
}
