/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 08:07:27 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 12:12:37 by frapp            ###   ########.fr       */
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

bool	unopend_quote(char *line)
{
	bool	except;
	int		i;
	char	quote_type;

	i = 0;
	except = false;
	quote_type = 0;
	while (line && line[i])
	{
		if (quote_type == line[i] && !except)
			quote_type = 0;
		else if (!except)
		{
			if (line[i] == '\'' || line[i] == '\"')
				quote_type = line[i];
		}
		if (!except && line[i] == '\\')
			except = true;
		else
			except = false;
		i++;
	}
	if (quote_type)
		return (true);
	return (false);
}

char	*ft_read_line(char *header)
{
	char	*line;
	char	*tmp;
	char	*tmp2;

	redisplay_prompt(true, true);
	tmp = NULL;
	if (!isatty(0))
	{
		tmp = get_next_line(0, false);
		line = ft_strtrim(tmp, "\n");
		ft_free((void **)&tmp);
	}
	else
		line = readline(header);
	while (unopend_quote(line))
	{
		if (!isatty(0))
		{
			tmp = get_next_line(0, false);
			if (!tmp)
				break ;
			tmp2 = ft_strtrim(tmp, "\n");
			ft_strjoin_inplace(&line, tmp2);
			free(tmp2);
		}
		else
		{
			tmp = readline("> ");
			if (!tmp)
				break ;
			ft_strjoin_inplace(&line, tmp);
		}
		ft_free((void **)&tmp);
		line_counter();
	}
	redisplay_prompt(true, false);
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
