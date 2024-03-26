/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 06:03:37 by frapp             #+#    #+#             */
/*   Updated: 2024/03/26 22:36:52 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

// for append_arg()
static t_arg	*get_cur_arg(t_arg *head_arg)
{
	t_arg	*cur;

	if (!head_arg)
		cur = ft_calloc(1, sizeof(t_arg));
	else
	{
		cur = head_arg;
		while (cur->next)
			cur = cur->next;
		cur->next = ft_calloc(1, sizeof(t_arg));
		cur = cur->next;
	}
	return (cur);
}

// returns the head of the token list, NULL on malloc fail
// assums the given list to be the head
t_arg	*append_arg(t_parser *parser, t_arg *head_arg)
{
	t_arg	*cur;

	if (!parser)
		return (NULL);
	cur = get_cur_arg(head_arg);
	if (!cur)
		return (free_arg_list(head_arg), NULL);
	if (!head_arg)
		head_arg = cur;
	cur->name = ft_calloc(1, sizeof(t_token_list));
	if (!cur->name)
		return (free_arg_list(head_arg), NULL);
	cur->name->token = parser->token;
	parser->token = NULL;
	if (parser->rest_name)
	{
		cur->name->next = extract_token_list(parser->rest_name, RECURSIVE_CALL);
		if (!cur->name->next)
			return (free_arg_list(head_arg), NULL);
	}
	cur->type = cur->name->token->type;
	return (head_arg);
}

static char	*handle_env_var_astparser(char *dollar_str, int *index)
{
	char	*return_str;
	char	*env_var;

	*index += name_len(dollar_str + 1) + 1;
	env_var = ft_strndup(dollar_str + 1, name_len(dollar_str + 1));
	if (!env_var)
		return (NULL);
	return_str = get_env_value(NULL, env_var, 0, 0);
	if (!return_str)
		return_str = ft_calloc(1, 1);
	return (free(env_var), return_str);
}

static char	*parser_expand_dollar(char *dollar_str, int *index)
{
	char	*return_str;

	if (!*(dollar_str + 1))
		return ((*index)++, ft_strdup("$"));
	else if (*(dollar_str + 1) == '?')
		return_str = get_last_exit_str();
	else if (*(dollar_str + 1) == '$')
		return_str = ft_itoa(ft_pid(0));
	else if (ft_isdigit(*(dollar_str + 1)))
		return ((*index)++, ft_strdup("$"));
	else if (name_len(dollar_str + 1) == 0)
		return ((*index)++, ft_strdup("$"));
	else if (ft_isalpha(*(dollar_str + 1)) || *(dollar_str + 1) == '_')
		return (handle_env_var_astparser(dollar_str, index));
	else
		return ((*index)++, ft_strdup(""));
	*index += 2;
	return (return_str);
}

char	*parser_expand_line(char *line)
{
	char	*new_line;
	int		i;
	char	*temp;

	new_line = ft_calloc(1, 1);
	if (!new_line)
		return (free(line), NULL);
	i = -1;
	while (line[++i] && line[i] != '\n')
	{
		if (line[i] != '$')
		{
			if (!ft_strjoin_inplace_char(&new_line, line[i]))
				return (free(line), NULL);
			continue ;
		}
		temp = parser_expand_dollar(line + i, &i);
		if (!temp)
			return (free(new_line), free(line), NULL);
		if (!ft_strjoin_inplace(&new_line, temp))
			return (free(temp), free(line), NULL);
		free(temp);
	}
	return (free(line), ft_strjoin_inplace_char(&new_line, '\n'), new_line);
}
