/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:36:06 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/25 09:55:39 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

/*
if shlvl is unset or non-numeric -> new subterminal has SHLVL=1.
if shlvl is negative -> new subshell has SHLVL=0.
*/
static bool	is_not_numeric(char *str)
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

static void	ft_update_shlvl(int shlvl_index, char ***env)
{
	char	*before;
	int		num;
	char	*after;

	num = -1;
	before = get_env_value(*(env), "SHLVL", 0, 0);
	if (shlvl_index == 0 || is_not_numeric(before))
	{
		*env = new_env_list_after_add("SHLVL=1", *env, false);
		free(before);
		return ;
	}
	if (*before != '-')
		num = ft_atoi(before);
	free(before);
	after = ft_itoa(num + 1);
	ft_update_env("SHLVL", after, *env);
	free(after);
}

char	**ft_initialize_env(char **base_env)
{
	int		i;
	char	**ret;
	int		shlvl_index;
	char	*tmp;
	char	buffer[PATH_MAX + 1];

	i = 0;
	shlvl_index = 0;
	while (base_env && base_env[i])
		i++;
	ret = ft_calloc((i + 60), sizeof(char *));
	if (ret == NULL)
		return (NULL);
	i = -1;
	while (base_env && base_env[++i])
		ret[i] = ft_strdup(base_env[i]);
	i = 0;
	while (base_env && base_env[i] && ft_strncmp(ret[i], "SHLVL=", 6))
		shlvl_index = i++;
	ft_update_shlvl(shlvl_index, &ret);
	tmp = ft_strjoin("PWD=", getcwd(buffer, PATH_MAX));
	ret = new_env_list_after_add(tmp, ret, false);
	tmp = ft_strjoin("_=", getenv("_"));
	ret = new_env_list_after_add(tmp, ret, false);
	return (free(tmp), ret);
}

// // THE PREVIOUS VERSION THAT I DELETE THE OLDPWD IN ENV LIST LIKE BASH

// static void	ft_clear_oldpwd(char ***env, bool keep)
// {
// 	*env = new_env_list_after_delete("OLDPWD", *env);
// 	if (keep == true)
// 		*env = new_env_list_after_add("OLDPWD", *env);
// }

// char	**ft_initialize_env(char **base_env, bool keep_oldpwd)
// {
// 	int		i;
// 	char	**ret;
// 	int		shlvl_index;

// 	i = 0;
// 	shlvl_index = 0;
// 	while (base_env[i])
// 		i++;
// 	ret = ft_calloc((i + 1), sizeof(char *));
// 	if (ret == NULL)
// 		return (NULL);
// 	ret[i] = NULL;
// 	i = -1;
// 	while (base_env[++i])
// 	{
// 		ret[i] = ft_strdup(base_env[i]);
// 		if (ft_strncmp(ret[i], "SHLVL=", 6) == 0)
// 			shlvl_index = i;
// 	}
// 	ft_update_shlvl(shlvl_index, &ret);
// 	ft_clear_oldpwd(&ret, keep_oldpwd);
// 	return (ret);
// }
