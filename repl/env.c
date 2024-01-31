/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 07:19:22 by frapp             #+#    #+#             */
/*   Updated: 2024/01/31 13:31:48 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	init_env(t_env *new_env, t_ast *ast, t_ast *sub_process)
{
	if (!ast)
		return ;
	if (sub_process)
	{
		new_env->pid = ast->env->pid;
		new_env->banned_vars = ast->env->banned_vars;
		new_env->extra_vars = ast->env->banned_vars;
		new_env->last_dir = ast->env->last_dir;
		new_env->last_exit_status = ast->env->last_exit_status;
		sub_process->env = new_env;
	}
	else
	{
		new_env->banned_vars = NULL;
		new_env->extra_vars = NULL;
		new_env->last_dir = NULL;
		new_env->last_exit_status = &(ast->exit_status);
		ast->env = new_env;
	}
	
}

void	free_env(t_env *env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	if (env->extra_vars)
	{
		while (env->extra_vars + i++)
		{
			my_free((void **)&((env->extra_vars[i]).name));
			my_free((void **)&((env->extra_vars[i]).val));
		}
		free(env->extra_vars);
	}
	i = 0;
	if (env->banned_vars)
	{
		while (env->banned_vars + i)
		{
			my_free((void **)&((env->banned_vars[i]).name));
			my_free((void **)&((env->banned_vars[i]).val));
		}
		free(env->banned_vars);
	}
}

void	bann_env(t_ast *ast)
{
	t_env	*env;

	env = ast->env;
	if (!env)
	{
		printf("bann_env error\n");
		exit(0);
	}
}

