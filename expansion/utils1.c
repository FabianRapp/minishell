/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:00:00 by frapp             #+#    #+#             */
/*   Updated: 2024/03/24 23:01:11 by mevangel         ###   ########.fr       */
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
	ft_free((void **)&(token->str_data));
	env_var = get_env_value(NULL, token->old_data, 0, 0);
	if (errno)
		return (ERROR);
	if (!env_var)
		token->str_data = ft_strdup("");
	else
		token->str_data = env_var;
	if (!token->str_data)
		return (ERROR);
	return (SUCCESS);
}

t_result	pidreq_to_literal_token(t_shared_data *env, t_token *token)
{
	token->type = LITERAL;
	token->str_data = ft_itoa(env->main_pid);
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

t_result	merge_literals(t_token_list *node)
{
	t_token_list	*to_free;

	while (node)
	{
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
		node = node->next;
	}
	return (SUCCESS);
}
