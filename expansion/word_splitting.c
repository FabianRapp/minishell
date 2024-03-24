/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_splitting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:33:33 by frapp             #+#    #+#             */
/*   Updated: 2024/03/24 22:37:04 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static t_token_list	*create_new_node_wordsplit(char *data, char *old_data)
{
	t_token_list	*new;

	new = ft_calloc(1, sizeof(t_token_list));
	if (!new)
		return (NULL);
	new->token = ft_calloc(1, sizeof(t_token));
	if (!new->token)
		return (free_token_list(new), NULL);
	new->token->type = LITERAL;
	new->token->str_data = ft_strdup(data);
	if (!new->token->str_data)
		return (free_token_list(new), NULL);
	new->token->old_data = ft_strdup(old_data);
	if (!new->token->old_data)
		return (free_token_list(new), NULL);
	if (contains_non_white_spcace(data))
		new->token->type = LITERAL;
	else
		new->token->type = WHITE_SPACE;
	return (new);
}

static t_result	word_split_baseclean(t_token *old, char **arr,
	t_token_list *next, t_token_list *new)
{
	if (new)
		free_token_list(new);
	free_token(old);
	ft_free_2darr(arr);
	free_token_list(next);
	return (ERROR);
}

static t_result	handle_end(int i, t_token *old, t_token_list **list, char **arr)
{
	ft_free_2darr(arr);
	if (old && ft_strlen(old->str_data)
		&& ft_iswhitespace(old->str_data[ft_strlen(old->str_data) - 1]))
	{
		return (insert_whitespace_end(list));
	}
	(void)i;
	return (SUCCESS);
}

t_result	word_splitting(t_token_list **list)
{
	char			**arr;
	int				i;
	t_token			*old;
	t_token_list	*next;
	t_token_list	*new;

	if (!*list)
		return (SUCCESS);
	old = (*list)->token;
	next = (*list)->next;
	arr = ft_split_fn(old->str_data, ft_iswhitespace);
	if (!ft_free((void **)list) || !arr)
		return (word_split_baseclean(old, arr, next, NULL), errno_to_result());
	i = 0;
	while (arr[i])
	{
		new = create_new_node_wordsplit(arr[i], old->old_data);
		if (!new || ((i++ || ft_iswhitespace(*(old->str_data)))
				&& insert_whitespace_before(&new) == ERROR))
			return (word_split_baseclean(old, arr, next, new));
		add_token_back_node(list, new);
	}
	if (handle_end(i, old, list, arr) == ERROR)
		return (word_split_baseclean(old, NULL, next, NULL));
	return (add_token_back_node(list, next), free_token(old), SUCCESS);
}
