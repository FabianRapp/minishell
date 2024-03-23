/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_wildcards_main.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:49:22 by frapp             #+#    #+#             */
/*   Updated: 2024/03/24 00:08:13 by frapp            ###   ########.fr       */
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
	new_token->str_data = next_file_name(cur_dir, w_para->prefix);
	while (new_token->str_data && !errno
		&& !matches_wildcard(new_token->str_data, w_para))
	{
		free(new_token->str_data);
		new_token->str_data = next_file_name(cur_dir, w_para->prefix);
	}
	if (!new_token->str_data || ft_strlen(new_token->str_data) == 0)
	{
		free_token(new_token);
		return (ft_free((void **) &(new_node)), NULL);
	}
	return (new_node);
}

void	expand_wildcard_node_exit(t_wildcard_node_expasion this_data,
	t_token_list *node)
{
	if (node)
	{
		free_token(node->token);
		node->token = NULL;
	}
	if (node && this_data.w_head)
	{
		*node = *(this_data.w_head);
		free(this_data.w_head);
	}
	if (this_data.cur_dir)
		closedir(this_data.cur_dir);
	ft_free((void **)&(this_data.w_str));
	clean_wildcard_data(&(this_data.w_para));
	if (this_data.cur)
		this_data.cur->next = this_data.base_next;
}

t_result	expand_wildcard_node(t_token_list *node)
{
	t_wildcard_node_expasion		this_data;
	static t_wildcard_node_expasion	init_data = {NULL, NULL, NULL, NULL, NULL, {NULL, NULL, NULL}};
	char	*tmp;

	this_data = init_data;
	this_data.w_str = ft_strdup(node->token->str_data);
	tmp = ft_strnstr(node->token->str_data, "3}{!", ft_strlen(node->token->str_data));
	if (tmp)
		ft_memmove(tmp, tmp + 4, ft_strlen(tmp + 4) + 1);
	this_data.base_next = node->next;
	if (fill_wildcard_data(this_data.w_str, &(this_data.w_para)) == ERROR)
		return (free(this_data.w_str), ERROR);
	this_data.cur_dir = opendir(".");
	if (errno)
		return (free(this_data.w_str), clean_wildcard_data(&(this_data.w_para)), ERROR);
	this_data.w_head = next_wildcard_token
		(this_data.cur_dir, &(this_data.w_para));
	if (!this_data.w_head)
		return (free(this_data.w_str), clean_wildcard_data(&(this_data.w_para)),
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

t_result	trim_identifiers(t_token_list *name, bool set_this_true)
{
	char	*tmp;
	char	**to_edit;

	if (set_this_true)
		to_edit = &(name->token->str_data);
	else
		to_edit = &(name->token->old_data);
	tmp = ft_strstrtrim(*to_edit, "1}{");
	free(*to_edit);
	*to_edit = tmp;
	tmp = ft_strstrtrim(tmp, "2}{");
	free(*to_edit);
	*to_edit = tmp;
	tmp = ft_strstrtrim(tmp, "3}{!");
	free(*to_edit);
	*to_edit = tmp;
	tmp = ft_strstrtrim(tmp, "3}{");
	free(*to_edit);
	*to_edit = tmp;
	if (!*to_edit)
		return (ERROR);
	return (trim_identifiers(name, false));
}

t_result	wildcards(t_token_list *name)
{
	char		*data_str;
	char		*tmp;

	data_str = NULL;
	while (name
		&& name->token
		&& name->token->type != T_EOF)
	{
		ft_free((void **)&data_str);
		if (name && name->token)
			data_str = ft_strdup(name->token->str_data);
		while (data_str && ft_strchr(data_str, '*'))
		{
			if (ft_strchr(data_str, '*') - data_str >= 3 && *(ft_strchr(data_str, '*') - 1) == '{'
				&& *(ft_strchr(data_str, '*') - 2) == '}')
			{
				name->token->type = LITERAL;
				expand_wildcard_node(name);
				if (trim_identifiers(name, true) == ERROR)
				{// TODO ERROR
				}
				break ;
			}
			tmp = ft_strdup(ft_strchr(data_str, '*') + 1);
			free(data_str);
			data_str = tmp;
		}
		name = name->next;
	}
	free(data_str);
	if (errno)
		return (ERROR);
	return (SUCCESS);
}
