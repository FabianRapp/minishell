/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_interpreted_strs.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 03:37:23 by frapp             #+#    #+#             */
/*   Updated: 2024/01/27 04:13:59 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/parser.h"

// 
char	*expand_interpreted_str(char	*str)
{
	int		i;
	char	*new_str;
	char	*env_var;

	new_str = ft_calloc(1, 1);
	if (!new_str)
		return (cleanup(), NULL);
	i = 0;
	while (str[i])
	{
		while (str[i] != '$' || !str[i + 1] || (!ft_isalpha(str[i + 1]) && str[i + 1] != '_' && str[i + 1] != '$'))
		{
			ft_strjoin_inplace_char(&new_str, str[i++]);
			if (!new_str)
				return (cleanup(), NULL);
		}
		if (str[i + 1] == '$') // $$ for somereason is an env var that is terminated by everything
		{
			ft_strjoin_inplace(&new_str , getenv("$$"));
		}
		else if (str[i] == '$' && ft_isdigit(str[i + 1])) // completly ignored by bash
			continue ;
		else if (str[i] == '$' && (ft_isalpha(str[i+1]) || str[i + 1] != '_'))
		{
			env_var = ft_strndup(str + i + 1 , name_len(str + i + 1));
			if (!env_var)
				return (cleanup(), NULL);
			ft_strjoin_inplace(&new_str , getenv(env_var));
			free(env_var);
		}
		if (!new_str)
			return (cleanup(), NULL);
	}
	return (new_str);
}

t_token_list	*expand_token_list(t_token_list *list)
{
	t_token_list	*head;

	head = list;
	while (list && list->token->type != T_EOF)
	{
		if (list->token->type == INTERPRETED)
		{
			list->token->str_data = expand_interpreted_str(list->token->str_data);
			if (!list->token->str_data)
				return (cleanup(), head);
		}
		else if(0)
		{
			// other token types
		}
		list = list->next;
	}
	return (head);
}

bool	expand_arg_list(t_arg *args)
{
	while (args && args->type != T_EOF)
	{
		if (!expand_token_list(args->name))
			return (false);
		args = args->next;
	}
	return (true);
}

void	*expand_interpreteted_strs(t_ast *ast)
{
	if (!ast)
		return (NULL);
	if (!is_redir(ast->type) && ast->type != COMMAND)
	{
		if (ast->left)
		{
			if (!expand_interpreteted_strs(ast->left))
				return (cleanup(), NULL);
		}
		if (ast->right)
		{
			if (!expand_interpreteted_strs(ast->right))
				return (cleanup(), NULL);
		}
	}
	else
	{
		if (ast->redir_in)
		{
			if (!expand_arg_list(ast->redir_in))
				return(cleanup(), NULL);
		}
		if (ast->redir_out)
		{
			if (!expand_arg_list(ast->redir_out))
				return(cleanup(), NULL);
		}
		if (ast->arg)
		{
			if (!expand_arg_list(ast->arg))
				return(cleanup(), NULL);
		}
		return (ast);
	}
	return (ast);
}
