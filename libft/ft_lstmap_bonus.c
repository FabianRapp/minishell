/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 16:24:54 by frapp             #+#    #+#             */
/*   Updated: 2023/10/09 18:08:20 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*current_new;
	t_list	*head;

	if (!lst)
		return (lst);
	head = (t_list *)ft_calloc(1, sizeof(t_list));
	current_new = head;
	while (current_new)
	{
		current_new->content = f(lst->content);
		lst = lst->next;
		if (!lst)
			break ;
		current_new->next = (t_list *)ft_calloc(1, sizeof(t_list));
		current_new = current_new->next;
		if (!current_new)
			ft_lstclear(&head, del);
	}
	return (head);
}
