/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 13:01:55 by frapp             #+#    #+#             */
/*   Updated: 2024/02/23 16:19:13 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

char	*expand_dollar(char *dollar_str, t_env *env, int *index)
{
	char	*env_var;
	char	*str;

	if (!*(dollar_str + 1))
	{
		*index += 1;
		return (ft_strdup("$"));
	}
	else if (*(dollar_str + 1) == '?')
	{
		*index += 2;
		return (get_last_exit_str());
	}
	else if (*(dollar_str + 1) == '$')
	{
		*index += 2;
		return (ft_itoa(env->main_pid));
	}
	else if (ft_isdigit(*(dollar_str + 1)))
	{
		*index += 2;
		return (ft_strdup(""));
	}
	else if (ft_isalpha(*(dollar_str + 1)) || *(dollar_str + 1) == '_')
	{
		*index += name_len(dollar_str + 1) + 1;
		env_var = ft_strndup(dollar_str + 1 , name_len(dollar_str + 1));
		if (!env_var)
			return (cleanup("expand_interpreted_str"), NULL);
		str = ft_strdup(getenv(env_var));
		free(env_var);
		return (str);
	}
	else //
	{
		printf("bug in expand_dollar(): what is this char: %c\n", *dollar_str);
		printf("and what is this: %c\n", dollar_str[1]);
		exit(0);// debug
		*index += 1;
		return (ft_strdup(""));
	}
}

bool	expand_interpreted(t_token *token, t_env *env)
{
	char	*data_str;
	char	*temp;
	int		index;

	data_str = token->str_data;
	token->str_data = NULL;
	index = 0;
	while (data_str[index])
	{
		if (data_str[index] != '$')
			ft_strjoin_inplace_char(&token->str_data, data_str[index++]);
		else
		{
			temp = expand_dollar(data_str + index, env, &index);
			if (!temp)
			{//malloc fail
			}
			ft_strjoin_inplace(&token->str_data, temp);
			free(temp);
			if (!token->str_data)
			{//malloc fail
			}
		}
	}
	token->type = LITERAL;
	free(data_str);
	return (true);
}

// copies the data of the nodes into their parent node
// removes the lst node (while keeping its data in the prev. node)
// and removes the data of the first node
t_token_list	*move_nodes_ahead(t_token_list *list, bool set_this_true)
{
	if (list && set_this_true)
		free_token(list->token);
	if (!list || !list->next)
	{
		if (list)
			free(list);
		return (NULL);
	}
	list->token = list->next->token;
	if (!list->next->next)
	{
		free(list->next);
		list->next = NULL;
		return (list);
	}
	list->next = move_nodes_ahead(list->next, false);
	return (list);
}

// intended to remove white space, also removes T_EOF and DUMMY_COMMAND,
// nothing else besides these and LITERALS(left over) should be in the
// list at this point
// errno has to be checked to check for malloc fail
t_token_list	*remove_non_literals(t_token_list *list)
{
	t_token_list	*head;
	t_token_list	*last;
	bool			last_was_whitespace;

	head = list;
	last = NULL;
	last_was_whitespace = true;
	while (list)
	{
		if (list->token->type != LITERAL)
		{
			if (list->token->type != WHITE_SPACE && list->token->type != DUMMY_COMMAND && list->token->type != WILDCARD)
			{
				printf("DEBUG remove_non_literals: this token type is unexpected: %s\n", type_to_str_type(list->token->type));
				exit(0);
			}
			if (list->token->type == WILDCARD)
			{
				if (!last_was_whitespace)
				{
					last->token->type = WILDCARD;
					if (!ft_strjoin_inplace_char(&(last->token->str_data), '*'))
						return (list);
					last->next = list->next;
					free_token(list->token);
					free(list);
					list = last;
				}
				last_was_whitespace = false;
			}
			else if (list->token->type != DUMMY_COMMAND)
			{
				last_was_whitespace = true;
				if (list == head)
				{
					list = move_nodes_ahead(list, true);
					head = list;
				}
				else
				{
					list = move_nodes_ahead(list, true);
					last->next = list;
				}
			}
			else
				last_was_whitespace = false;
		}
		else
			last_was_whitespace = false;
		last = list;
		if (list)
			list = list->next;
	}
	return (head);
}
