/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_splitting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:33:33 by frapp             #+#    #+#             */
/*   Updated: 2024/03/02 17:54:31 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static t_token	*create_new_token(char *data, char *old_data)
{
	t_token	*new;

	new = ft_calloc(1, sizeof(t_token));
	if (!new)
		return (NULL);
	new->type = LITERAL;
	new->str_data = ft_strdup(data);
	if (!new->str_data)
		return (free(new), NULL);
	new->old_data = ft_strdup(old_data);
	if (!new->old_data)
		return (free_token(new), NULL);
	new->type = LITERAL;
	return (new);
}

static t_result	word_split_baseclean(t_token *old, char **arr,
	t_token_list *next, t_token *new)
{
	if (new)
		free_token(new);
	free_token(old);
	free_str_ar(arr);
	free_token_list(next);
	return (ERROR);
}

t_result	word_splitting(t_token_list **list)
{
	char			**arr;
	int				i;
	t_token			*new;
	t_token			*old;
	t_token_list	*next;

	if (!*list)
		return (SUCCESS);
	old = (*list)->token;
	next = (*list)->next;
	arr = ft_split_fn(old->str_data, ft_iswhitespace);
	if (!my_free((void **)list) || !arr || !*arr)
		return (word_split_baseclean(old, arr, next, NULL), errno_to_result());
	i = 0;
	while (arr[i])
	{
		if (i && insert_whitespace_end(list) == ERROR)
			return (word_split_baseclean(old, arr, next, NULL));
		new = create_new_token(arr[i], old->old_data);
		if (!new || add_token_back(list, new) == ERROR)
			return (word_split_baseclean(old, arr, next, new));
		i++;
	}
	add_token_back_node(list, next);
	return (free_str_ar(arr), free_token(old), SUCCESS);
}
