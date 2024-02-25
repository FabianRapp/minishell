/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_wildcards_main.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:49:22 by frapp             #+#    #+#             */
/*   Updated: 2024/02/25 05:59:22 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../expansion.h"

t_token_list	*next_wildcard_token(DIR *cur_dir, t_wildcard_parameters *w_para)
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
	while (new_token->str_data && !errno && !matches_wildcard(new_token->str_data, w_para))
	{
		free(new_token->str_data);
		new_token->str_data = next_file_name(cur_dir);
	}
	if (!new_token->str_data)
	{
		free_token(new_token);
		return (my_free((void **) &(new_node)), NULL);
	}
	return (new_node);
}

t_token_list	*new_list_node(void)
{
	t_token_list	*new_node;
	t_token			*new_token;

	new_node = ft_calloc(1, sizeof(t_token_list));
	if (!new_node)
	{
		return (NULL);
	}
	new_token = ft_calloc(1, sizeof(t_token));
	if (!new_token)
	{
		return (free(new_node), NULL);
	}
	new_node->token = new_token;
	return (new_node);
}

typedef struct s_wildcard_node_expansion
{
	t_token_list			*base_next;
	t_token_list			*cur;
	DIR						*cur_dir;
	t_wildcard_parameters	w_para;
	char					*w_str;
	t_token_list			*w_head;
}	t_wildcard_node_expasion;

void	expand_wildcard_node_exit(t_wildcard_node_expasion this_data, t_token_list *node)
{
	free_token(node->token);
	*node = *(this_data.w_head);
	*node = *(this_data.w_head);
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
	fill_wildcard_data(this_data.w_str, &(this_data.w_para));
	if (errno)
		return (ERROR);
	this_data.cur_dir = opendir(".");
	if (errno)
		return (clean_wildcard_data(&(this_data.w_para)), ERROR);
	this_data.w_head = next_wildcard_token(this_data.cur_dir, &(this_data.w_para));
	if (!this_data.w_head)
		return (clean_wildcard_data(&(this_data.w_para)),
			closedir(this_data.cur_dir), errno_to_result());
	this_data.cur = this_data.w_head;
	this_data.cur->next = next_wildcard_token(this_data.cur_dir, &(this_data.w_para));
	while (this_data.cur->next)
	{
		this_data.cur = this_data.cur->next;
		this_data.cur->next = next_wildcard_token(this_data.cur_dir, &(this_data.w_para));
	}
	expand_wildcard_node_exit(this_data, node);
	return (errno_to_result());
}

t_result	wildcards_expand_name(t_token_list *name)
{
	while (name)
	{
		if (ft_strchr(name->token->str_data, '*'))
		{
			expand_wildcard_node(name);
		}
		name = name->next;
	}
	if (errno)
		return (ERROR);
	return (SUCCESS);
}
