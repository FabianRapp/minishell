/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:36:06 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/19 01:17:24 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/eval.h"

void	ft_env(t_ast *ast)
{
	int		i;
	char	**tmp;
	char	*path;

	path = get_env_value(*(ast->shared_data->envs), "PATH");
	if (!path)
	{
		print_error(true, "env", NULL, "No such file or directory");
		ft_cur_exit(ast, 127);
		free(path);
		return ;
	}
	free(path);
	if (ast && ast->shared_data->envs)
		tmp = *(ast->shared_data->envs);
	else
		tmp = *(get_env_list(NULL));
	i = -1;
	while (tmp[++i])
		printf("%s\n", tmp[i]);
	ft_cur_exit(ast, 0);
}
