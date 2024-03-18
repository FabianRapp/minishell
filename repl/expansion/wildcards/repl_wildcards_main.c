/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_wildcards_main.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:49:22 by frapp             #+#    #+#             */
/*   Updated: 2024/03/18 02:49:13 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../expansion.h"

t_token_list	*next_wildcard_token(DIR *cur_dir,
	t_wildcard_parameters *w_para)
{
	t_token			*new_token;
	t_token_list	*new_node;

	new_token = ft_calloc(1, sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_node = ft_calloc(1, sizeof(t_token_list));
	if (!new_node)
		return (free(new_token), NULL);
	new_token->type = LITERAL;
	new_node->token = new_token;
	new_token->str_data = next_file_name(cur_dir);
	while (new_token->str_data && !errno
		&& !matches_wildcard(new_token->str_data, w_para))
	{
		free(new_token->str_data);
		new_token->str_data = next_file_name(cur_dir);
	}
	if (!new_token->str_data)
	{
		free_token(new_token);
		return (ft_free((void **) &(new_node)), NULL);
	}
	return (new_node);
}

void	expand_wildcard_node_exit(t_wildcard_node_expasion this_data,
	t_token_list *node)
{
	free_token(node->token);
	*node = *(this_data.w_head);
	closedir(this_data.cur_dir);
	clean_wildcard_data(&(this_data.w_para));
	this_data.cur->next = this_data.base_next;
}

t_result	expand_wildcard_node(t_token_list *node)
{
	t_wildcard_node_expasion	this_data;

	this_data.w_str = node->token->str_data;
	this_data.base_next = node->next;
	if (fill_wildcard_data(this_data.w_str, &(this_data.w_para)) == ERROR)
		return (ERROR);
	this_data.cur_dir = opendir(".");
	if (errno)
		return (clean_wildcard_data(&(this_data.w_para)), ERROR);
	this_data.w_head = next_wildcard_token
		(this_data.cur_dir, &(this_data.w_para));
	if (!this_data.w_head)
		return (clean_wildcard_data(&(this_data.w_para)),
			closedir(this_data.cur_dir), errno_to_result());
	this_data.cur = this_data.w_head;
	this_data.cur->next = next_wildcard_token(
			this_data.cur_dir, &(this_data.w_para));
	while (this_data.cur->next)
	{
		this_data.cur = this_data.cur->next;
		this_data.cur->next = next_wildcard_token(
				this_data.cur_dir, &(this_data.w_para));
	}
	expand_wildcard_node_exit(this_data, node);
	return (errno_to_result());
}

t_result	wildcards(t_token_list *name)
{
	char		*tmp;
	char		*data_str;

	while (name)
	{
		// if (name->token->type == WILDCARD && ft_strchr(name->token->str_data, '*'))
		data_str = name->token->str_data;
		// /if (name->token && data_str && ft_strchr(data_str, '*'))
		while (name->token && data_str && ft_strchr(data_str, '*'))
		{
			//printf("dat str: %s\n", data_str);
			if (ft_strchr(data_str, '*') - data_str >= 2 && *(ft_strchr(data_str, '*') - 1) == '{'
				&& *(ft_strchr(data_str, '*') - 2) == '}')
			{
				// tmp = ft_strstrtrim(name->token->str_data, "}{");
				// printf("before: %s\n after: %s\n", name->token->str_data, tmp);
				// free(name->token->str_data);
				// name->token->str_data = tmp;
				// if (!name->token->str_data)
				// {// todo error
				// }
				name->token->type = LITERAL;
				expand_wildcard_node(name);
				tmp = ft_strstrtrim(name->token->str_data, "}{");
				//printf("before: %s\n after: %s\n", name->token->str_data, tmp);
				free(name->token->str_data);
				name->token->str_data = tmp;
				if (!name->token->str_data)
				{// todo error
				}
			}
			else
				data_str = ft_strchr(data_str, '*') + 1;
		}
		name = name->next;
	}
	if (errno)
		return (ERROR);
	return (SUCCESS);
}
