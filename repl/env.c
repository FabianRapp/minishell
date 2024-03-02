/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 07:19:22 by frapp             #+#    #+#             */
/*   Updated: 2024/03/02 20:09:20 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

bool	init_env(t_env *new_env, char **base_env)
{
	if (!new_env)
		return (false);
	if (!base_env)
		return (false);
	new_env->main_process = true;
	new_env->stop_execution = false;
	return (true);
}
