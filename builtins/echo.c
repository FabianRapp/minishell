/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 08:00:49 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/23 04:46:10 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static bool	is_the_n_option(char *arg)
{
	int	i;

	i = 1;
	if (arg[0] != '-')
		return (false);
	while (arg[i] && arg[i] == 'n')
		i++;
	if (arg[i] == '\0')
		return (true);
	else
		return (false);
}

static bool	print_with_or_without_space(t_arg *cur_arg, char *str_value)
{
	if (cur_arg->next)
		printf("%s ", str_value);
	else
		printf("%s", str_value);
	return (true);
}

t_result	expand_home(t_arg *cur_arg, t_ast *ast)
{
	char	*str_value;

	str_value = cur_arg->name->token->str_data;
	if (str_value && *str_value == '~' && !*(str_value + 1))
	{
		free(str_value);
		str_value = get_env_value(NULL, "HOME", 0, 0);
		cur_arg->name->token->str_data = str_value;
	}
	return (set_errno_as_exit(ast, false));
}

t_result	ft_echo(t_ast *ast, t_arg *cur_arg)
{
	char	*str_value;
	bool	no_new_line;
	bool	stop_check;

	no_new_line = false;
	stop_check = false;
	while (cur_arg && cur_arg->name->token->type != T_EOF)
	{
		if (expand_home(cur_arg, ast) == ERROR)
			return (ERROR);
		str_value = cur_arg->name->token->str_data;
		if (!stop_check && is_the_n_option(str_value))
			no_new_line = true;
		else if (str_value)
			stop_check = print_with_or_without_space(cur_arg, str_value);
		cur_arg = cur_arg->next;
	}
	if (no_new_line == false)
		printf("\n");
	return (ft_cur_exit(ast, 0));
}

t_result	ft_cap_echo(t_ast *ast, t_arg *cur_arg)
{
	char	*str;
	bool	no_new_line;

	ft_cur_exit(ast, 0);
	if (!cur_arg)
		return (printf("\n"), 0);
	no_new_line = false;
	str = cur_arg->name->token->str_data;
	if (*str && *str == '-' && *(str + 1) == 'n' && !*(str + 2))
	{
		no_new_line = true;
		cur_arg = cur_arg->next;
	}
	while (cur_arg && cur_arg->name->token->type != T_EOF)
	{
		if (expand_home(cur_arg, ast) == ERROR)
			return (ERROR);
		str = cur_arg->name->token->str_data;
		if (str)
			print_with_or_without_space(cur_arg, str);
		cur_arg = cur_arg->next;
	}
	if (no_new_line == false)
		printf("\n");
	return (SUCCESS);
}
