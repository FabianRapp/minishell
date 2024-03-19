/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 08:00:49 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/19 07:09:10 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/eval.h"

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

int	ft_echo(t_ast *ast)
{
	t_arg	*cur_arg;
	char	*str_value;
	bool	no_new_line;
	bool	stop_check;

	ft_cur_exit(ast, 0);
	cur_arg = ast->arg;
	if (!cur_arg)
		return (printf("\n"), 0);
	no_new_line = false;
	stop_check = false;
	while (cur_arg && cur_arg->name->token->type != T_EOF)
	{
		str_value = cur_arg->name->token->str_data;
		if (!stop_check && is_the_n_option(str_value))
			no_new_line = true;
		else if (str_value)
			stop_check = print_with_or_without_space(cur_arg, str_value);
		cur_arg = cur_arg->next;
	}
	if (no_new_line == false)
		printf("\n");
	return (0);
}
