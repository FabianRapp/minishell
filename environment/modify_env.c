/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:36:06 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/26 16:06:34 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static bool	if_already_in_env(char **env, char *str_to_add, bool plus)
{
	char	*var_to_add;
	char	*line_name;
	int		i;

	i = 0;
	if (!env)
		return (false);
	var_to_add = get_env_var_name(str_to_add);
	line_name = get_env_var_name(env[i]);
	while (env[i] && ft_strcmp(line_name, var_to_add))
	{
		free(line_name);
		line_name = get_env_var_name(env[++i]);
	}
	free(line_name);
	if (!env[i])
		return (free(var_to_add), false);
	if (!plus)
	{
		free(env[i]);
		env[i] = ft_strdup(str_to_add);
	}
	else
		ft_strjoin_inplace(&(env[i]), ft_strchr(str_to_add, '=') + 1);
	return (free(var_to_add), true);
}

static char	*ft_rm_the_plus_from_str(char *str)
{
	char	*var_name;
	char	*value;

	var_name = get_env_var_name(str);
	value = ft_substr(str, ft_strlen(var_name) + 1, ft_strlen(str)
			- ft_strlen(var_name) - 1);
	free(str);
	return (ft_strjoin_free_both(var_name, value));
}

char	**new_env_list_after_add(char *str_to_add, char **env, bool plus)
{
	char	**env_before;
	int		i;
	char	**new;

	i = 0;
	env_before = env;
	if (if_already_in_env(env, str_to_add, plus))
		return (env_before);
	if (ft_strchr(str_to_add, '+'))
		str_to_add = ft_rm_the_plus_from_str(str_to_add);
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
