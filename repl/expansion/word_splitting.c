/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_splitting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:33:33 by frapp             #+#    #+#             */
/*   Updated: 2024/02/23 15:19:51 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

// copies the given node to a new which is inserted after it
// empties the given node and makes it a white space node
// (after this the given node should be white space and the next (data) should be the old given node)
t_token_list	*insert_whitespace_before(t_token_list *before)
{
	t_token_list	*new;

	new = ft_calloc(1, sizeof(t_token_list));
	if (!new)
	{//malloc fail
	}
	new->token = before->token;
	new->next = before->next;
	before->next = new;
	before->token = ft_calloc(1, sizeof(t_token));
	if (!before->token)
	{// malloc fail
	}
	before->token->type = WHITE_SPACE;
	return (new);
}

t_token	*next_literal_token(char *str, int *index)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL);
	while (str[*index] && !ft_iswhitespace(str[*index]))
	{
		ft_strjoin_inplace_char(&(token->str_data), str[*index]);
		if (!token->str_data)
			return (free(token), NULL);
		(*index)++;
	}
	token->type = LITERAL;
	return (token);
}

// fills the old node with the new data and appends a new node with the old data
bool	insert_next_literal_node(t_token_list *list, char *str, int *index)
{
	t_token_list	*new;

	if (!list)
		return (false);
	new = ft_calloc(1, sizeof(t_token_list));
	if (!new)
		return (false);
	new->token = list->token;
	list->token = next_literal_token(str, index);
	if (!new->token)
		return (free(new), NULL);
	new->next = list->next;
	list->next = new;
	return (true);
}

//TODO should clean list incase of malloc fail
t_token_list	*word_splitting(t_token_list *list)
{
	int				index;
	t_token_list	*head;
	char			*str;
	t_token_list	*last;

	if (!list)
		return (NULL);
	head = list;
	index = 0;
	str = list->token->str_data;
	last = NULL;
	while (str[index])
	{
		if (ft_iswhitespace(str[index]))
		{
			if (!insert_whitespace_before(list))
			{//malloc fail
			}
			while (str[index] && ft_iswhitespace(str[index]))
				index++;
			last = list;
			list = list->next;
		}
		if (str[index])
		{
			if (!insert_next_literal_node(list, str, &index))
			{//handle error
			}
			last = list;
			list = list->next;
		}
	}
	if (last)
		last->next = list->next;
	free_token(list->token);
	free(list);
	if (head != list)
		return (head);
	return (NULL);
}
