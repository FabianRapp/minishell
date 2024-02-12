/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 07:19:22 by frapp             #+#    #+#             */
/*   Updated: 2024/02/12 17:10:51 by frapp            ###   ########.fr       */
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

t_env	clone_env(t_env *base)
{
	t_env	clone;
	int		i;

	clone.main_process = false;
	clone.main_pid = 0;
	clone.vars = NULL;
	if (!base)
		return (clone);
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
	clone.main_pid = base->main_pid;
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
	printf("pid: %d\n", env->main_pid);
}

void	update_var(t_env *env, char *new_data)
{
	t_env_var	new_var;
	int			i;
	t_env_var	*new_vars;

	i = 0;
	extract_env(new_data, &new_var);//TODO malloc fail
	while (env->vars[i].name)
	{
		if (!ft_strcmp(env->vars[i].name, new_var.name))
		{
			env->vars[i].val = new_var.val;
			return ;
		}
	}
	new_vars = ft_calloc(i + 1, sizeof(t_env_var));// TODO malloc fail
	ft_memcpy(new_vars, env->vars, sizeof(t_env_var) * i);
	new_vars[i].name = new_var.name;
	new_vars[i].val = new_var.val;
	free(env->vars);
	env->vars = new_vars;
}


// // one for each process level needed
// // 0 length size are not allowed
// // format: <name>=<value>
// // length = strlen("<name>=<value>")
// void	var_manger(t_env *env)
// {
// 	char	*total_var_data;
// 	char	*var_name;
// 	char	*var_val;
// 	int		size;

// 	// TODO exit via signal?
// 	while (1)
// 	{
		
// 		// if ready to read data
// 		{
// 			read(env->size_name[READ], &size, sizeof(int));
// 			total_var_data = ft_calloc(size + 1, sizeof(char));
// 			read(env->name_fd[READ], total_var_data, size);
// 			update_var(t_env, total_var_data);
// 		}
// 	}
// }
