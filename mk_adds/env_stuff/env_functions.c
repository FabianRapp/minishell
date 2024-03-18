/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:36:06 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/18 03:31:52 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/eval.h"

char	***get_env_list(char ***set_new_env)
{
	static char	***env = NULL;

	if (set_new_env)
		env = set_new_env;
	return (env);
}

char	*get_env_var_name(char *line)
{
	int		len;
	char	*var_name;
	
	len = 0;
	if (!line)
		return (NULL);
	while (line[len] != '=' && line[len] != '\0')
		len++;
	var_name = ft_substr(line, 0, len);
	return (var_name);
}

// allocates
char	*get_env_value(char **env, char *var_name)
{
	int		i;
	char	*value;
	char	*line_name;

	if (!env)
		env = *(get_env_list(NULL));
	i = -1;
	while(env[++i])
	{
		line_name = get_env_var_name(env[i]);
		if (ft_strcmp(line_name, var_name) == 0)
			break ;
	}
	if (env[i] == NULL) //means the var_name doesn't exist in env
		return (NULL);
	value = ft_substr(env[i], ft_strlen(var_name) + 1, ft_strlen(env[i])
		- ft_strlen(var_name) - 1);
	if (value == NULL)
		return (NULL);
	return (value);
}

static char	**if_already_in_env(char **env, char *str_to_add, int *add)
{
	char	*var_to_add_name;
	char	*cur_name;
	int		i;
	char	**ret;

	i = 0;
	var_to_add_name = get_env_var_name(str_to_add);
	ret = env;
	cur_name = get_env_var_name(env[i]);
	while (env[i] && ft_strcmp(var_to_add_name, cur_name))
	{
		free(cur_name);
		cur_name = get_env_var_name(env[++i]);
	}
	if (env[i]) //which means that the var exists already
	{
		if (ft_strchr(str_to_add, '='))
			ret = delete_env_var(var_to_add_name, &env);
		else
			*add = 0;
	}
	return (free(var_to_add_name), ret);
}

char	**add_env_var(char *str_to_add, char **env)
{
	char	**env_before;
	int		i;
	char	**new;
	int		add;

	i = 0;
	add = 1;
	env_before = env;
	env_before = if_already_in_env(env_before, str_to_add, &add);
	if (add == 0)
		return (env_before);
	while (env_before[i])
		i++;
	new = (char **)malloc((i + 2) * sizeof(char *));
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

char	**delete_env_var(char *var_to_rm, char ***arr_ptr)
{
	char	**env_before;
	int		i;
	char	**new;
	int		del_index;
	char	*cur_name;

	env_before = *(arr_ptr);
	i = 0;
	cur_name = get_env_var_name(env_before[i]);
	while (env_before[i] && ft_strcmp(var_to_rm, cur_name))
	{
		free(cur_name);
		cur_name = get_env_var_name(env_before[++i]);
	}
	if (env_before[i] == NULL)
		return (env_before);
	new = (char **)malloc(ft_strarr_size(env_before) * sizeof(char *));
	if (new == NULL)
		return (NULL);
	del_index = i;
	i = -1;
	while (++i < del_index)
		new[i] = ft_strdup(env_before[i]);
	while (env_before[++i])
		new[i - 1] = ft_strdup(env_before[i]);
	new[i - 1] = NULL;
	ft_free_2darr(env_before);
	return (new);
}

void	ft_update_env_var(char *var_name, char *new_value, char **env)
{
	int		i;
	char	*half;
	
	i = 0;
	while (env[i] && ft_strncmp(var_name, env[i], ft_strlen(var_name)))
		i++;
	free(env[i]);
	half = ft_strjoin(var_name, "=");
	env[i] = ft_strjoin(half, new_value);
	free(half);
}

/*
if shlvl is unset or non-numeric -> new subterminal has SHLVL=1.
*/
bool	is_not_numeric(char *str)
{
	if (*str == '+' || *str == '-')
		str++;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (true);
		str++;
	}
	return (false);
}

void	ft_update_shlvl(int shlvl_index, char ***env)
{
	char	*before;
	int		num;
	char	*after;

	num = -1;
	before = get_env_value(*(env), "SHLVL");
	if (shlvl_index == 0 || is_not_numeric(before))//for the case that the SHLVL was unset in env
	{
		*env = add_env_var("SHLVL=1", *env);
		free(before);
		return ;
	}
	if (*before != '-')
		num = ft_atoi(before);
	free(before);
	after = ft_itoa(num + 1);
	ft_update_env_var("SHLVL", after, *env);
}

char	**ft_initialize_our_env(char **base_env)
{
	int		i;
	char	**ret;
	int		shlvl_index;

	i = 0;
	shlvl_index = 0;
	while (base_env[i])
		i++;
	ret = malloc((i + 1) * sizeof(char *));
	if (ret == NULL)
		return (NULL);
	ret[i] = NULL;
	i = -1;
	while (base_env[++i])
	{
		ret[i] = ft_strdup(base_env[i]);
		if (ft_strncmp(ret[i], "SHLVL=", 6) == 0)
			shlvl_index = i;
	}
	ft_update_shlvl(shlvl_index, &ret);
	return (ret);
}
