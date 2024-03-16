/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:36:06 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/16 22:14:20 by frapp            ###   ########.fr       */
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

	if (ast && ast->envs)
		tmp = *(ast->envs);
	else
		tmp = *(get_env(NULL));
	i = -1;
	while (tmp[++i])
		printf("%s\n", tmp[i]);
}
