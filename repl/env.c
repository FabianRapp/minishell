/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 07:19:22 by frapp             #+#    #+#             */
/*   Updated: 2024/03/05 07:41:32 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

// bool	extract_env(char *str, t_env_var *var)
// {
// 	char		**split;

// 	if (!str || !var)
// 		return (false);
// 	split = ft_split(str, '=');
// 	if (!split)
// 		return (false);
// 	if (split[0] && split[1])
// 	{
// 		var->name = split[0];
// 		var->val = split[1];
// 	}
// 	if (split)
// 		free(split);
// 	return (true);
// }

bool	init_env(t_env *new_env, char **base_env)
{
	if (!new_env)
		return (false);
	if (!base_env)
		return (false);
	new_env->stop_execution = false;
	return (true);
}
