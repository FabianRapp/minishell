/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_strs1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 03:37:23 by frapp             #+#    #+#             */
/*   Updated: 2024/01/27 23:44:33 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/parser.h"

char	*env_var_to_str(char *env_var)
{
	char	*not_free_able;

	if (ft_strlen(env_var) <= 0)
		return (NULL);
	not_free_able = getenv(env_var);
	if (!not_free_able)
		return (NULL);
	if (!*not_free_able)
		return (NULL);
	return (ft_strdup(getenv(env_var)));
}

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
		if (str[i] == '$' && !str[i + 1])
			break ;
		while ((str[i] && str[i] != '$') || (str[i] == '$' && (!str[i + 1] || (!ft_isalpha(str[i + 1]) && str[i + 1] != '_' && str[i + 1] != '$'))))
		{
			ft_strjoin_inplace_char(&new_str, str[i]);
			i++;
			//printf("new str: %s\n", new_str);
			if (!new_str)
				return (cleanup(), NULL);
		}
		if (!str[i])
			break ;
		if (str[i] == '$' && str[i + 1] == '$') // $$ for somereason is an env var that is terminated by everything
		{
			ft_strjoin_inplace(&new_str , getenv("$$"));
			i++;
		}
		else if (str[i] == '$' && ft_isdigit(str[i + 1])) // completly ignored by bash
		{
			i++;
			continue ;
		}
		else if (str[i] == '$' && (ft_isalpha(str[i+1]) || str[i + 1] == '_'))
		{
			env_var = ft_strndup(str + i + 1 , name_len(str + i + 1));
			if (!env_var)
				return (cleanup(), NULL);
			ft_strjoin_inplace(&new_str , getenv(env_var));
			free(env_var);
			i += name_len(str + i + 1) + 1;
		}
		else //
		{
			printf("bug in expand_interpreted_str(): what is this char: %c\n", str[i]);
			printf("and what is this: %c\n", str[i + 1]);
			i++;
		}
		if (!new_str)
			return (cleanup(), NULL);
	}
	my_free((void **)&str);
	return (new_str);
}

t_token_list	*word_to_literals(t_token_list *list)
{
	char			**all_literals;
	int				i;
	t_token_list	*temp;

	if (list->token->type != WORD)
		return (list);
	all_literals = ft_split_fn(list->token->str_data, ft_iswhitespace);
	if (!all_literals)
		return (cleanup(), list);
	if (!all_literals[1])
		return (free_str_ar(all_literals), list);
	my_free((void **)&(list->token->str_data));
	list->token->str_data = all_literals[0];
	list->token->type = LITERAL;
	i = 1;
	while (all_literals[i])
	{
		temp = list->next;
		list->next = ft_calloc(1, sizeof(t_token_list));
		if (!list->next)
		{
			list->next = temp;
			return (cleanup(), list);
		}
		list = list->next;
		list->next = temp;
		list->token = ft_calloc(1, sizeof(t_token));
		if (!list->token)
			return (cleanup(), list);
		list->token->str_data = all_literals[i++];
		list->token->type = LITERAL;
	}
	free(all_literals);
	return (list);
}

t_token_list	*expand_token_list(t_token_list *list)
{
	t_token_list	*head;
	char			*temp;

	head = list;
	while (list && list->token->type != T_EOF)
	{
		if (list->token->type == INTERPRETED)
		{
			list->token->str_data = expand_interpreted_str(list->token->str_data);
			if (!list->token->str_data)
				return (cleanup(), head);
			list->token->type = LITERAL;
		}
		else if (list->token->type == ENV_VAR)
		{
			temp = env_var_to_str(list->token->str_data);
			my_free((void **)&(list->token->str_data));
			list->token->str_data = temp;
			if (temp)
			{
				list->token->type = WORD;
				list = word_to_literals(list);
				if (!list)
					return (cleanup(), head);
			}
			else
			{
				list->token->type = VOID;
			}
		}
		else if (0)
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

t_arg	*move_additional_literals_to_args(t_token_list *name, t_arg *args, bool is_name)
{
	t_arg	*new_args;
	t_arg	*head;

	if (!name)
		return (args);
	head = NULL;
	new_args = NULL;
	while (name && name->next)
	{
		if (name->token->type == LITERAL && name->next && name->next->token->type == LITERAL)
		{
			if (new_args)
			{
				new_args->next = ft_calloc(1, sizeof(t_arg));
				new_args = new_args->next;
			}
			else
			{
				new_args = ft_calloc(1, sizeof(t_arg));
				head = new_args;
			}
			if (!new_args)
			{
				return (cleanup(), args);
			}
			new_args->name = name->next;
			name->next = new_args->name->next;
			new_args->name->next = NULL;
			new_args->next = NULL;
		}
		else
			name = name->next;
	}
	if (!head)
		return (args);
	if (is_name)
	{
		new_args->next = args;
		return (head);
	}
	new_args->next = args->next;
	args->next = head;
	return (args);
}

t_arg	*fix_arg_literals(t_arg *args)
{
	t_arg	*head;
	t_arg	*last;

	if (!args)
		return (NULL);
	head = args;// if only one arg of T_EOF
	last = NULL;
	while (args && args->type != T_EOF)
	{
		args = move_additional_literals_to_args(args->name, args, false);
		{
			///need malloc protection
		}
		if (last)
			last->next = args;
		else
			head = args;;
		last = args;
		args = args->next;
	}
	return (head);
}

#define EXPAND_FULL_AST // EXPAND_FULL_AST: for testing (not usable later on since vars have to be evaluated on runtime)
// expands the current ast nodes env vars and interpreted strs (if the first)
void	*expand_strs(t_ast *ast)
{
	if (!ast)
		return (NULL);
	#ifdef EXPAND_FULL_AST
	if (!is_redir(ast->type) && ast->type != COMMAND)
	{
		if (ast->left)
		{
			if (!expand_strs(ast->left))
				return (cleanup(), NULL);
		}
		if (ast->right)
		{
			if (!expand_strs(ast->right))
				return (cleanup(), NULL);
		}
	}
	else
	#endif
	{
		if (ast->name)
		{
			if (!expand_token_list(ast->name))
				return (cleanup(), NULL);
			ast->arg = move_additional_literals_to_args(ast->name, ast->arg, true);
			// needs malloc protection
			//if (!ast->arg)
				//return (cleanup(), ast);
		}
		if (ast->redir_in)
		{
			if (!expand_arg_list(ast->redir_in))
				return(cleanup(), NULL);
			ast->redir_in = fix_arg_literals(ast->redir_in);
			// needs malloc protection
		}
		if (ast->redir_out)
		{
			if (!expand_arg_list(ast->redir_out))
				return(cleanup(), NULL);
			ast->redir_out = fix_arg_literals(ast->redir_out);
			// needs malloc protection
		}
		if (ast->arg)
		{
			if (!expand_arg_list(ast->arg))
				return(cleanup(), NULL);
			ast->arg = fix_arg_literals(ast->arg);
			// needs malloc protection
		}
		return (ast);
	}
	return (ast);
}
