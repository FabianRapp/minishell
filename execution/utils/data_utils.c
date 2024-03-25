/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 02:38:43 by frapp             #+#    #+#             */
/*   Updated: 2024/03/25 00:27:04 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

int	count_args(t_arg *args)
{
	int		count;

	count = 0;
	while (args && args->type != T_EOF)
	{
		count++;
		args = args->next;
	}
	return (count);
}

// do not free the strings in argv, they are wiht in the tokens and
// are hadndeled by the maincleanup
void	fill_args(t_ast *ast, char *argv[], int type)
{
	t_arg	*args;
	int		i;

	if (type == ARGS)
		args = ast->arg;
	else
		return ;
	i = 0;
	while (args && args->type != T_EOF)
	{
		argv[i++] = args->name->token->str_data;
		args = args->next;
	}
	argv[i] = NULL;
}

// check errno when calling this
char	*extract_command_name(char *path)
{
	char	*name;
	int		i;
	char	**arr;

	if (ft_strncmp("/", path, 1) && ft_strncmp(".", path, 1) && ft_strncmp("..", path, 2))
		return (ft_strdup(path));
	arr = ft_split(path, '/');
	if (!arr)
		return (NULL);
	i = 0;
	while (arr[i])
	{
		i++;
	}
	if (!i)
		return (ft_free_2darr(arr), NULL);
	name = arr[i - 1];
	arr[i - 1] = NULL;
	ft_free_2darr(arr);
	return (name);
}
