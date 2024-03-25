/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 00:33:41 by frapp             #+#    #+#             */
/*   Updated: 2024/03/25 00:44:23 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

t_token	*new_dummy_token(void)
{
	t_token	*dummy;

	dummy = ft_calloc(1, sizeof(t_token));
	if (!dummy)
		return (NULL);
	dummy->type = DUMMY_COMMAND;
	return (dummy);
}

t_result	add_token_back(t_token_list **list, t_token *token)
{
	t_token_list	*new_node;

	new_node = ft_calloc(1, sizeof(t_token_list));
	if (!new_node)
		return (ERROR);
	new_node->token = token;
	if (!*list)
	{
		*list = new_node;
		return (SUCCESS);
	}
	while ((*list)->next)
	{
		list = &((*list)->next);
	}
	(*list)->next = new_node;
	return (SUCCESS);
}

t_result	insert_whitespace_end(t_token_list **list)
{
	t_token	*new;

	new = ft_calloc(1, sizeof(t_token));
	if (!new)
		return (ERROR);
	new->type = WHITE_SPACE;
	if (add_token_back(list, new) == ERROR)
		return (free(new), ERROR);
	return (SUCCESS);
}

t_result	insert_whitespace_before(t_token_list **head)
{
	t_token_list	*new;

	new = ft_calloc(1, sizeof(t_token_list));
	if (!new)
		return (ERROR);
	new->token = ft_calloc(1, sizeof(t_token));
	if (!new->token)
		return (free(new), ERROR);
	new->token->type = WHITE_SPACE;
	add_token_node_front(head, new);
	return (SUCCESS);
}
