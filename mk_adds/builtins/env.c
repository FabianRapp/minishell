/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:36:06 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/18 22:05:49 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
env (alone): will show a list of environment varaibles in  the format:
			VARIABLE=value

but it can also be used like:
env VARIABLE=value some_command
		|-> in this case, it runs the command in the modified environment (tmporary)
			where the environmental variable VARIABLE is added
*/

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
		ft_fprintf(2, "%s: env: No such file or directory\n", SHELL_NAME);
		set_last_exit(127);
		ast->exit_status = 127;
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
