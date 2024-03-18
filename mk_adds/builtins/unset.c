/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:34:24 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/18 23:06:04 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/eval.h"

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
		if (arg_is_valid(str_value, ast, "unset"))
		{
			*(ast->shared_data->envs) = delete_env_var(str_value,
				ast->shared_data->envs);
			*(ast->shared_data->env_exp) = delete_env_var(str_value,
				ast->shared_data->env_exp);
		}
		cur_arg = cur_arg->next;
	}
}
