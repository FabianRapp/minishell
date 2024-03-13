/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:36:06 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/11 15:22:36 by mevangel         ###   ########.fr       */
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

//! for some reason, this prints the following weird: 
/*
minishell-$: env | grep "HI"
hereee
minishell-$:
*/

#include "../../headers/minishell.h"
#include "../../headers/eval.h"

void	ft_env(t_ast *ast)
{
	int		i;
	char	**tmp;

	tmp = *(ast->envs);
	i = -1;
	while (tmp[++i])
		printf("%s\n", tmp[i]);
}
