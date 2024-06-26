/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 13:01:55 by frapp             #+#    #+#             */
/*   Updated: 2024/03/26 22:34:07 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static char	*expand_dollar(char *dollar_str, int *index, char **return_str)
{
	char	*env_var;

	if (!*(dollar_str + 1))
		return ((*index)++, ft_strdup("$"));
	else if (*(dollar_str + 1) == '?')
		*return_str = get_last_exit_str();
	else if (*(dollar_str + 1) == '$')
		*return_str = ft_itoa(ft_pid(0));
	else if (ft_isdigit(*(dollar_str + 1)))
		return ((*index)++, ft_strdup("$"));
	else if (name_len(dollar_str + 1) == 0)
		return ((*index)++, ft_strdup("$"));
	else if (ft_isalpha(*(dollar_str + 1)) || *(dollar_str + 1) == '_')
	{
		*index += name_len(dollar_str + 1) + 1;
		env_var = ft_strndup(dollar_str + 1, name_len(dollar_str + 1));
		if (!env_var)
			return (NULL);
		*return_str = get_env_value(NULL, env_var, 0, 0);
		return (free(env_var), *return_str);
	}
	else
		return ((*index)++, ft_strdup(""));
	*index += 2;
	return (*return_str);
}

t_result	expand_interpreted(t_token *token)
{
	char	*data_str;
	char	*temp;
	int		index;

	data_str = token->str_data;
	token->str_data = ft_calloc(1, 1);
	if (!token->str_data)
		return (ERROR);
	index = 0;
	while (data_str[index])
	{
		if (data_str[index] != '$')
			ft_strjoin_inplace_char(&token->str_data, data_str[index++]);
		else
		{
			temp = expand_dollar(data_str + index, &index, &temp);
			if (temp)
				ft_strjoin_inplace(&token->str_data, temp);
			free(temp);
		}
		if (!token->str_data)
			return (ERROR);
	}
	token->type = LITERAL;
	return (free(data_str), SUCCESS);
}

// copies the data of the nodes into their parent node
// removes the lst node (while keeping its data in the prev. node)
// and removes the data of the first node
// does not needed to be error checked
t_token_list	*move_nodes_ahead(t_token_list *list, bool set_this_true)
{
	if (!list)
		return (NULL);
	if (set_this_true)
	{
		free_token(list->token);
		list->token = NULL;
	}
	if (list->next)
	{
		list->token = list->next->token;
		list->next->token = NULL;
	}
	else
	{
		free(list);
		return (NULL);
	}
	list->next = move_nodes_ahead(list->next, false);
	return (list);
}

// intended to remove white space, also removes T_EOF and DUMMY_COMMAND,
// nothing else besides these and LITERALS(left over) should be in the
// list at this point
// does not needed to be error checked
// might case bugs if white more whitespace gets to this point? works now tho
t_token_list	*remove_non_literals(t_token_list *list)
{
	t_token_list	*head;
	t_token_list	*last;

	if (!list || errno)
		return (NULL);
	head = list;
	last = NULL;
	while (list && merge_literals(list))
	{
		if (list->token->type != LITERAL && list->token->type
			!= DUMMY_COMMAND && list->token->type != WILDCARD)
		{
			if (list == head)
			{
				list = move_nodes_ahead(list, true);
				head = list;
			}
			else
				last->next = list;
		}
		last = list;
		if (list)
			list = list->next;
	}
	return (head);
}

t_result	expansion_move_to_arg(t_arg **next_arg, t_token_list *list)
{
	t_arg	*new;

	while (list && list->next)
	{
		new = ft_calloc(1, sizeof(t_arg));
		if (!new)
			return (ERROR);
		new->name = list->next;
		list->next = list->next->next;
		new->name->next = NULL;
		add_arg_front(next_arg, new);
		next_arg = &((*next_arg)->next);
	}
	return (SUCCESS);
}
