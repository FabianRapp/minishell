/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_wildcards_main.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:49:22 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 15:16:32 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static t_token_list	*next_wildcard_token(DIR *cur_dir,
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

static void	expand_wildcard_node_exit(t_wildcard_node_expasion data,
	t_token_list *node)
{
	if (node)
	{
		free_token(node->token);
		node->token = NULL;
	}
	if (node && data.w_head)
	{
		*node = *(data.w_head);
		free(data.w_head);
	}
	if (data.cur_dir)
		closedir(data.cur_dir);
	ft_free((void **)&(data.w_str));
	clean_wildcard_data(&(data.w_para));
	if (data.cur)
		data.cur->next = data.base_next;
}

static t_result	expand_wildcard_node(t_token_list *node)
{
	t_wildcard_node_expasion		data;
	static t_wildcard_node_expasion	init_data = {NULL, NULL,
		NULL, NULL, NULL, {NULL, NULL, NULL}};

	data = init_data;
	data.w_str = ft_strdup(node->token->str_data);
	data.base_next = node->next;
	if (fill_wildcard_data(data.w_str, &(data.w_para)) == ERROR)
		return (free(data.w_str), ERROR);
	data.cur_dir = opendir(".");
	if (errno)
		return (free(data.w_str), clean_wildcard_data(&(data.w_para)), ERROR);
	data.w_head = next_wildcard_token(data.cur_dir, &(data.w_para));
	if (!data.w_head)
		return (free(data.w_str), clean_wildcard_data(&(data.w_para)),
			closedir(data.cur_dir), errno_to_result());
	data.cur = data.w_head;
	data.cur->next = next_wildcard_token(data.cur_dir, &(data.w_para));
	while (data.cur->next)
	{
		data.cur = data.cur->next;
		data.cur->next = next_wildcard_token(data.cur_dir, &(data.w_para));
	}
	return (expand_wildcard_node_exit(data, node), errno_to_result());
}

// returns true if the inner loop shouold break
static bool	check_and_expand(char *data_str, t_token_list *name)
{
	if (ft_strchr(data_str, '*') - data_str >= 3
		&& *(ft_strchr(data_str, '*') - 1) == '{'
		&& *(ft_strchr(data_str, '*') - 2) == '}')
	{
		name->token->type = LITERAL;
		expand_wildcard_node(name);
		trim_identifiers(name, true);
		return (true);
	}
	return (false);
}

t_result	wildcards(t_token_list *name)
{
	char		*data_str;
	char		*tmp;

	data_str = NULL;
	while (name && name->token && name->token->type != T_EOF)
	{
		ft_free((void **)&data_str);
		if (name && name->token)
			data_str = ft_strdup(name->token->str_data);
		while (data_str && ft_strchr(data_str, '*'))
		{
			if (check_and_expand(data_str, name) == true)
				break ;
			tmp = ft_strdup(ft_strchr(data_str, '*') + 1);
			free(data_str);
			data_str = tmp;
		}
		if (errno)
			return (free(data_str), ERROR);
		name = name->next;
	}
	if (errno)
		return (free(data_str), ERROR);
	return (free(data_str), SUCCESS);
}
