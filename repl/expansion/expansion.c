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
t_result	merge_literals(t_token_list *node)
{
	t_token_list	*to_free;

	while (node && node->token->type == LITERAL && node->next
		&& node->next->token->type == LITERAL)
	{
		if (!ft_strjoin_inplace(&(node->token->str_data),
				node->next->token->str_data))
		{
			return (ERROR);
		}
		to_free = node->next;
		node->next = node->next->next;
		free_token(to_free->token);
		free(to_free);
	}
	return (SUCCESS);
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

// check errno for error after calling this (from merge_literals())
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
		if (word_splitting(&list) == ERROR)
			return (list);
	}
	if (!list)
		return (NULL);
	if (list->next && list->next->token->type != T_EOF)
		list->next = expand_list_here_doc(env, list->next);
	if (errno)
		return (list);
	merge_literals(list);
	return (list);
}

// check errno for error after calling
t_token_list	*expand_list_normal(t_env *env, t_token_list *list)
{
	if (!list)
		return (NULL);
	if (list->token->type == INTERPRETED
		&& expand_interpreted(list->token, env) == ERROR)
		return (list);
	if (list->token->type == ENV_VAR && env_to_word_token(list->token) == ERROR)
		return (list);
	if (list->token->type == PID_REQUEST
		&& pidreq_to_literal_token(env, list->token) == ERROR)
		return (list);
	if (list->token->type == EXIT_STATUS_REQUEST)
	{
		list->token->str_data = get_last_exit_str();
		if (!list->token->str_data)
			return (list);
		list->token->type = LITERAL;
	}
	
	if (list->token->type == WORD && word_splitting(&list) == ERROR)
	{
		return (list);
	}
	if (!list)
		return (NULL);
	// t_token_list	*debug = list;
	// while (debug)
	// {
	// 	printf("debug: %s\n", debug->token->str_data);
	// 	debug = debug->next;
	// }
	
	if (list->token->type == WORD)
		list->token->type = LITERAL;
	list->next = expand_list_normal(env, list->next);
	if (errno)
	{
		printf("did failed here (%s)\n", strerror(errno));
		return (list);
	}
	merge_literals(list);
	
	return (list);
}

t_result	expansion_move_to_arg(t_arg **next_arg, t_token_list *list)
{
	t_arg	*new;

	while (list && list->next)
	{
		new = ft_calloc(1, sizeof(t_arg));
		if (!new)
			return (ERROR);
		new->name = list->next;
		list->next = list->next->next;
		new->name->next = NULL;
		add_arg_front(next_arg, new);
		next_arg = &((*next_arg)->next);
	}
	return (SUCCESS);
}

t_result	expand_name(t_ast *ast)
{
	if (!ast->name)
		return (SUCCESS);
	ast->name = expand_list_normal(ast->env, ast->name);
	if (errno)
		return (set_errno_as_exit(ast));
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
			return (set_errno_as_exit(ast));
		ast->name->token = new_dummy_token();
		if (!ast->name->token)
			return (set_errno_as_exit(ast));
		return (SUCCESS);
	}
	if (expansion_move_to_arg(&(ast->arg), ast->name) == ERROR)
		return (set_errno_as_exit(ast));
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
			return (set_errno_as_exit(ast));
		if (wildcards_expand_name(cur->name) == ERROR)
			return (set_errno_as_exit(ast));
		if (cur->name)
			cur->name = remove_non_literals(cur->name);
		if (errno)
			return (set_errno_as_exit(ast));
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
					// ast->arg = ast->arg->next;
					// free(cur);
					// cur = ast->arg;
		
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
			return (set_errno_as_exit(ast));
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
// expands the current ast nodes env vars and interpreted strs (if the first)
t_result	expansion(t_ast *ast)
{
	if (!ast)
		return (ERROR);
	if (ast->type != COMMAND)
		return (SUCCESS);
	if (!expand_name(ast))
		return (ERROR);
	if (expand_args(ast, &(ast->arg), true) == ERROR)
		return (ERROR);
	if (expand_redirs(ast) == ERROR)
		return (ERROR);
	return (SUCCESS);
}
