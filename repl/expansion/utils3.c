/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 23:07:05 by frapp             #+#    #+#             */
/*   Updated: 2024/03/09 07:53:38 by frapp            ###   ########.fr       */
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
