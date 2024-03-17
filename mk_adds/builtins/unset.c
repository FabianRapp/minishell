/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:34:24 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/17 19:23:57 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// the unset removes the environmental variable from the current shell session.

// if i do `unset randomvalue` nothing happens. no error no nothing.

#include "../../headers/minishell.h"
#include "../../headers/eval.h"

void	ft_unset(t_ast *ast)
{
	t_arg	*cur_arg;
	char	*str_value;
	int		res;

	res = 0;
	cur_arg = ast->arg;
	while (cur_arg && cur_arg->name->token->type != T_EOF)
	{
		str_value = cur_arg->name->token->str_data;
		if (arg_is_valid(str_value))
		{
			*(ast->shared_data->envs) = delete_env_var(str_value, ast->shared_data->envs);
			*(ast->shared_data->env_exp) = delete_env_var(str_value, ast->shared_data->env_exp);
		}
		// res = arg_is_valid(str_value);
		// if (res > 0)
		// 	*(ast->shared_data->envs) = delete_env_var(str_value, ast->shared_data->envs);
		// if (res == 1)
		// 	*(ast->shared_data->env_exp) = delete_env_var(str_value, ast->shared_data->env_exp);
		cur_arg = cur_arg->next;
	}
	ast->exit_status = 0;
	set_last_exit(0);
}
