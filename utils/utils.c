/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 08:07:27 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 09:51:02 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	add_token_back_node(t_token_list **list, t_token_list *new_node)
{
	if (!*list)
	{
		*list = new_node;
		return ;
	}
	while ((*list)->next)
	{
		list = &((*list)->next);
	}
	(*list)->next = new_node;
	return ;
}

void	add_token_node_front(t_token_list **head, t_token_list *new_list)
{
	t_token_list	*temp;

	temp = *head;
	*head = new_list;
	new_list->next = temp;
}

char	*ft_read_line(char *header)
{
	char	*line;
	char	*temp;

	redisplay_prompt(true, true);
	if (!isatty(0))
	{
		temp = get_next_line(0, false);
		line = ft_strtrim(temp, "\n");
		get_next_line(0, true);
		free(temp);
	}
	else
	{
		line = readline(header);
	}
	redisplay_prompt(true, false);
	line_counter();
	return (line);
}

int	name_len(char *str)
{
	int		len;

	if (!str)
		return (0);
	if (!*str || (*str != '_' && !ft_isalpha(*str)))
		return (0);
	len = 1;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
	{
		len++;
	}
	return (len);
}
