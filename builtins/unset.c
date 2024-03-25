/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:34:24 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/25 01:12:26 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static bool	arg_is_valid_unset(char *arg, t_ast *ast, char *cmd_name)
{
	int		i;
	char	*save;

	i = 0;
	save = arg;
	if (!arg)
		return (false);
	if (*arg == '-')
	{
		print_error(true, "unset", save, "invalid option");
		ft_fprintf(2, "no options supported\n");
		return (false);
	}
	while (*arg && *arg != '=')
	{
		if (!(ft_isprint((int) *arg)))
			return (ft_cur_exit(ast, 1), print_error_weird_quotes(true,
					cmd_name, save, "not a valid identifier"), false);
		arg++;
	}
	return (true);
}

void	ft_unset(t_ast *ast)
{
	t_arg	*cur_arg;
	char	*str_value;
	int		res;

	res = 0;
	cur_arg = ast->arg;
	ft_cur_exit(ast, 0);
	while (cur_arg && cur_arg->name->token->type != T_EOF)
	{
		str_value = cur_arg->name->token->str_data;
		if (arg_is_valid_unset(str_value, ast, "unset"))
		{
			*(ast->shared_data->envs) = new_env_list_after_delete(str_value,
					*(ast->shared_data->envs));
			*(ast->shared_data->env_exp) = new_env_list_after_delete(str_value,
					*(ast->shared_data->env_exp));
		}
		else
		{
			set_last_exit(2);
			ast->exit_status = 2;
			return ;
		}
		cur_arg = cur_arg->next;
	}
}
