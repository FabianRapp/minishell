/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:34:24 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/19 05:53:04 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/eval.h"

//!unset PATH gives segfault
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
			*(ast->shared_data->envs) = new_env_list_after_delete(str_value,
				*(ast->shared_data->envs));
			*(ast->shared_data->env_exp) = new_env_list_after_delete(str_value,
				*(ast->shared_data->env_exp));
		}
		cur_arg = cur_arg->next;
	}
}

 /* 
             TOTAL TEST COUNT: 935  TESTS PASSED: 847  LEAKING: 0 
                     STD_OUT: 63  STD_ERR: 19  EXIT_CODE: 46  
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 128   
                                     ✅ 2677   
*/
