/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:36:06 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/22 02:39:22 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/eval.h"

static char	**if_already_in_env(char **env, char *str_to_add, int *add)
{
	char	*var_to_add;
	char	*line_name;
	int		i;
	char	**ret;

	i = 0;
	var_to_add = get_env_var_name(str_to_add);
	ret = env;
	line_name = get_env_var_name(env[i]);
	while (env[i] && ft_strcmp(line_name, var_to_add))
	{
		free(line_name);
		line_name = get_env_var_name(env[++i]);
	}
	free(line_name);
	if (env[i])
	{
		if (ft_strchr(str_to_add, '='))
			ret = new_env_list_after_delete(var_to_add, env);
		else
			*add = 0;
	}
	return (free(var_to_add), ret);
}

char	**new_env_list_after_add(char *str_to_add, char **env)
{
	char	**env_before;
	int		i;
	char	**new;
	int		add;

	i = 0;
	add = 1;
	env_before = if_already_in_env(env, str_to_add, &add);
	if (add == 0)
		return (env_before);
	while (env_before[i])
		i++;
	new = (char **)ft_calloc((i + 2), sizeof(char *));
	if (new == NULL)
		return (NULL);
	new[i + 1] = NULL;
	i = -1;
	while (env_before[++i])
		new[i] = ft_strdup(env_before[i]);
	new[i] = ft_strdup(str_to_add);
	ft_free_2darr(env_before);
	return (new);
}

char	**new_env_list_after_delete(char *var_to_rm, char **env_before)
{
	int		i;
	char	**new;
	int		del_index;
	char	*line_name;

	i = 0;
	line_name = get_env_var_name(env_before[i]);
	while (env_before[i] && ft_strcmp(var_to_rm, line_name))
	{
		free(line_name);
		line_name = get_env_var_name(env_before[++i]);
	}
	if (env_before[i] == NULL)
		return (free(line_name), env_before);
	new = (char **)ft_calloc(ft_strarr_size(env_before) + 1, sizeof(char *));
	if (new == NULL)
		return (NULL);
	del_index = i;
	i = -1;
	while (++i < del_index)
		new[i] = ft_strdup(env_before[i]);
	while (env_before[++i])
		new[i - 1] = ft_strdup(env_before[i]);
	new[i - 1] = NULL;
	return (free(line_name), ft_free_2darr(env_before), new);
}

void	ft_update_env(char *var_name, char *new_value, char **env)
{
	int		i;
	char	*half;
	
	i = 0;
	while (env[i] && ft_strncmp(var_name, env[i], ft_strlen(var_name)))
		i++;
	if (!env[i])
		return ;
	free(env[i]);
	half = ft_strjoin(var_name, "=");
	env[i] = ft_strjoin(half, new_value);
	free(half);
}



// // static bool	is_not_in_env(char *var_name, char **env)
// // {
// // 	while (*env)
// // 	{
// // 		if (ft_strncmp(var_name, *env, ft_strlen(var_name)) == 0)
// // 			return (false);
// // 		env++;
// // 	}
// // 	return (true);
// // }

// void	ft_update_env(char *var_name, char *new_value, char **env)
// {
// 	int		i;
// 	char	*half;
// 	char	*var_str;

// 	i = 0;
// 	if (!env)
// 		return ;
		
// 	half = ft_strjoin(var_name, "=");
// 	var_str = ft_strjoin(half, new_value);
// 	free(half);
	
// 	env = new_env_list_after_add(var_str, env);
		
// 	// while (env[i] && ft_strncmp(var_name, env[i], ft_strlen(var_name)))
// 	// 	i++;
// 	// // if (env[i] == NULL)//which means that the var doesn't exist in the list
		
// 	// free(env[i]);
// 	// half = ft_strjoin(var_name, "=");
// 	// env[i] = ft_strjoin(half, new_value);
// 	// free(half);
// }
