/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_wildcards_main.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:49:22 by frapp             #+#    #+#             */
/*   Updated: 2024/02/23 16:25:10 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../expansion.h"

t_token_list	*next_wildcard_token(DIR *cur_dir, t_wildcard_data *w_data)
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
	if (errno || !new_token->str_data)
		return (free(new_token), free(new_node), NULL);
	while (new_token->str_data && !errno && !matches_wildcard(new_token->str_data, w_data))
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
		errno = 0;
		return (NULL);
	}
	new_token = ft_calloc(1, sizeof(t_token));
	if (!new_token)
	{
		errno = 0;
		return (free(new_node), NULL);
	}
	new_node->token = new_token;
	return (new_node);
}

// check errno for malloc or open/read error
void	expand_wildcard_node(t_token_list *node)
{
	t_token_list	*next;
	t_token_list	*cur;
	DIR				*cur_dir;
	t_wildcard_data	w_data;
	char			*w_str;
	t_token_list	*w_head;
	t_token_list	*new_whitespace;

	w_str = node->token->str_data;
	fill_wildcard_data(w_str, &w_data);
	if (errno)
		return ;
	next = node->next;
	cur_dir = opendir(".");
	if (errno)
		return ;
	w_head = next_wildcard_token(cur_dir, &w_data);
	if (!w_head)
	{
		node->token->type = LITERAL;
		clean_wildcard_data(&w_data);
		return ;
	}
	cur = w_head;
	cur->next = next_wildcard_token(cur_dir, &w_data);
	while (cur->next)
	{
		new_whitespace = new_list_node();
		if (!new_whitespace)
		{
			errno = 1;
			clean_wildcard_data(&w_data);
			return ;
		}
		new_whitespace->token->type = WHITE_SPACE;
		new_whitespace->next = cur->next;
		cur->next = new_whitespace;
		cur = new_whitespace->next;
		cur->next = next_wildcard_token(cur_dir, &w_data);
	}
	cur->next = next;
	free_token(node->token);
	*node = *w_head;
	free(w_head);
	closedir(cur_dir);
	clean_wildcard_data(&w_data);
}

t_result	wildcards_expand_name(t_token_list *name)
{
	while (name)
	{
		if (name->token->type == WILDCARD)
		{
			expand_wildcard_node(name);
		}
		name = name->next;
	}
	if (errno)
		return (ERROR);
	return (SUCCESS);
}

t_result	wildcards_expand_arg(t_arg *arg)
{
	while (arg)
	{
		wildcards_expand_name(arg->name);
		arg = arg->next;
	}
	return (SUCCESS);
}

t_result	wildcards_expand_redir(t_redir *redir)
{
	while (redir)
	{
		wildcards_expand_arg(redir->arg);
		redir = redir->next;
	}
	return (SUCCESS);
}

t_result	expand_wildcards(t_ast *ast)
{
	if (wildcards_expand_name(ast->name) == ERROR || errno)
		return (ERROR);
	if (wildcards_expand_arg(ast->arg) == ERROR || errno)
		return (ERROR);
	if (wildcards_expand_redir(ast->redir) == ERROR || errno)
		return (ERROR);
	return (SUCCESS);
}
