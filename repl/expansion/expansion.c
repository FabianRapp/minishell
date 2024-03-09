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

// check errno for error after calling
t_token_list	*expand_list(t_env *env, t_token_list *list)
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
	list->next = expand_list(env, list->next);
	if ((list->token->type == WORD && word_splitting(&list) == ERROR) || !list)
		return (list);
	merge_literals(list);
	return (list);
}

t_result	expand_name(t_ast *ast)
{
	if (!ast->name)
		return (SUCCESS);
	ast->name = expand_list(ast->env, ast->name);
	if (errno)
		return (set_errno_as_exit(ast));
	if (ast->name)
		ast->name = remove_non_literals(ast->name);
	if (wildcards(ast->name) == ERROR)
		return (ERROR);
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

t_result	expand_args(t_ast *ast, t_arg **base_arg, bool here_doc)
{
	t_arg	*cur;
	t_arg	*last;
	int		flag;

	cur = *base_arg;
	if (!cur)
		return (SUCCESS);
	last = NULL;
	while (cur)
	{
		if (!here_doc)
			cur->name = expand_list(ast->env, cur->name);
		cur->name = remove_non_literals(cur->name);
		if (errno || (!here_doc && wildcards(cur->name) == ERROR))
			return (set_errno_as_exit(ast));
		flag = check_empty_arg(last, &cur, ast, base_arg);
		if (flag == RETURN_NOW)
			return (SUCCESS);
		if (flag == CONTINUE)
			continue ;
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
			if (expand_args(ast, &(cur->arg), false) == ERROR)
				return (ERROR);
		}
		else
		{
			if (expand_args(ast, &(cur->arg), true) == ERROR)
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
	if (expand_args(ast, &(ast->arg), false) == ERROR)
		return (ERROR);
	if (expand_redirs(ast) == ERROR)
		return (ERROR);
	return (SUCCESS);
}
