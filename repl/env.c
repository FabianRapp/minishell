/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 07:19:22 by frapp             #+#    #+#             */
/*   Updated: 2024/02/01 09:52:53 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"


void	free_env(t_env *env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env->vars && env->vars[i].name)
	{
		my_free((void **) &((env->vars)[i].name));
		my_free((void **) &((env->vars)[i++].name));
	}
	my_free((void **)&(env->vars));
	env->last_exit_status = NULL;
}

bool	extract_env(char *str, t_env_var *var)
{
	char		**split;

	if (!str || !var)
		return (false);
	split = ft_split(str, '=');
	if (!split)
		return (false);
	if (split[0] && split[1])
	{
		var->name = split[0];
		var->val = split[1];
	}
	if (split)
		free(split);
	return (true);
}

bool	init_env(t_env *new_env, char **base_env)
{
	int	i;

	if (!new_env)
		return (false);
	new_env->vars = NULL;
	if (!base_env)
		return (false);
	i = 0;
	while (base_env[i])
		i++;
	new_env->vars = ft_calloc(i + 1, sizeof(t_env_var));
	if (!new_env->vars)
		return (false);
	i = 0;
	while (base_env[i])
	{
		if (!extract_env(base_env[i], (new_env->vars) + i))
			return (free_env(new_env), false);
		i++;
	}
	new_env->main_process = true;
	return (true);
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

t_env	clone_env(t_env *base)
{
	t_env	clone;
	int		i;

	clone.main_process = false;
	clone.last_exit_status = NULL;
	clone.pid = 0;
	clone.vars = NULL;
	if (!base)
		return (clone);
	clone.last_exit_status = base->last_exit_status;
	i = 0;
	while (base->vars && base->vars[i].name)
		i++;
	if (!i)
		return (clone);
	clone.vars = ft_calloc(i + 1, sizeof(t_env_var));
	if (!clone.vars)
		return (clone);
	i = 0;
	while (base->vars && base->vars[i].name)
	{
		clone.vars[i].name = ft_strdup(base->vars[i].name);
		if (!clone.vars[i].name)
			return (free_env(&clone), clone);
		clone.vars[i].val = ft_strdup(base->vars[i].name);
		if (!clone.vars[i++].val)
			return (my_free((void **)(clone.vars[i].name)), free_env(&clone), clone);
	}
	clone.pid = base->pid;
	return (clone);
}

//debug
void	print_env(t_env *env)
{
	if (!env)
	{
		printf("no env\n");
		return ;
	}
	for (int i = 0; (env->vars)[i].name; i++)
	{
		printf("name: %s\n", (env->vars)[i].name);
		printf("val: %s\n", (env->vars)[i].val);
	}
	if ((env->last_exit_status))
		printf("last exit status: %d\n", *(env->last_exit_status));
	printf("pid: %d\n", env->pid);
}
