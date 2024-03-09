/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 23:07:05 by frapp             #+#    #+#             */
/*   Updated: 2024/03/09 02:23:47 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

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

// // check errno for error after calling this (from merge_literals())
// t_token_list	*expand_list_here_doc(t_env *env, t_token_list *list)
// {
// 	if (list->token->type == INTERPRETED)
// 		list->token->type = LITERAL;
// 	if (list->token->type == ENV_VAR
// 		|| list->token->type == PID_REQUEST
// 		|| list->token->type == EXIT_STATUS_REQUEST)
// 	{
// 		if (add_dollar(list->token) == ERROR)
// 			return (list);
// 	}
// 	if (list->token->type == WORD)
// 	{
// 		if (word_splitting(&list) == ERROR)
// 			return (list);
// 	}
// 	if (!list)
// 		return (NULL);
// 	if (list->next && list->next->token->type != T_EOF)
// 		list->next = expand_list_here_doc(env, list->next);
// 	if (errno)
// 		return (list);
// 	merge_literals(list);
// 	return (list);
// }
