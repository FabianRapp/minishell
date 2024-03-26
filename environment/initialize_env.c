/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:36:06 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/26 16:29:47 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

/*
*	if shlvl is unset or non-numeric -> new subterminal has SHLVL=1.
*	if shlvl is negative -> new subshell has SHLVL=0.
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
	if (shlvl_index == -1 || is_not_numeric(before))
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
	char	*tmp;
	char	buffer[PATH_MAX + 1];

	i = 0;
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
		i++;
	ft_update_shlvl(i - 1, &ret);
	tmp = ft_strjoin("PWD=", getcwd(buffer, PATH_MAX));
	ret = new_env_list_after_add(tmp, ret, false);
	free(tmp);
	tmp = ft_strjoin("_=", getenv("_"));
	ret = new_env_list_after_add(tmp, ret, false);
	return (free(tmp), ret);
}
