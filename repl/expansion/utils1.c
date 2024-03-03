/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:00:00 by frapp             #+#    #+#             */
/*   Updated: 2024/03/04 00:32:44 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

void	add_arg_front(t_arg **head, t_arg *new)
{
	t_arg	*temp;

	temp = *head;
	*head = new;
	new->next = temp;
}

t_result	env_to_word_token(t_token *token)
{
	char	*env_var;

	token->type = WORD;
	my_free((void **)&(token->str_data));
	env_var = getenv(token->old_data);
	errno = 0;
	if (!env_var)
	{
		token->type = WHITE_SPACE;
	}
	else
		token->str_data = ft_strdup(env_var);
	if (!token->str_data)
		return (ERROR);
	return (SUCCESS);
}

t_result	pidreq_to_literal_token(t_env *env, t_token *token)
{
	token->type = LITERAL;
	token->str_data = ft_itoa(env->main_pid);
	if (!token->str_data)
		return (ERROR);
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

// utils for expand_args
// returns true if expanding args is finsihed
int	check_empty_arg(t_arg *last, t_arg **cur,
		t_ast *ast, t_arg **base_arg)
{
	if ((*cur)->name)
		return (GO_ON);
	if (last)
	{
		last->next = (*cur)->next;
		free(*cur);
		(*cur) = last->next;
	}
	else
	{
		if (*base_arg)
		{
			(*cur) = ast->arg->next;
			free(ast->arg);
			*base_arg = (*cur);
		}
		else
			return (RETURN_NOW);
	}
	return (CONTINUE);
}
