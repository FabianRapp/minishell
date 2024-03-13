/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:34:24 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/11 16:26:45 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// the unset removes the environmental variable from the current shell session. 

// if i do `unset randomvalue` nothing happens. no error no nothing.

#include "../../headers/minishell.h"
#include "../../headers/eval.h"

void	ft_unset(t_ast *ast)
{
	char	**env;
	int		i;
	
	env = *(ast->envs);
	i = -1;
	printf("value i got here is: %s$\n", get_env_value(ast, "COLORTERM"));
	// while (env[++i])
	// 	printf("%s\n", env[i]);
	
}