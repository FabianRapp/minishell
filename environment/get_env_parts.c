/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_parts.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:36:06 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/22 22:44:22 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

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
	if (var_name == NULL)
		return (NULL);
	return (var_name);
}

char	***get_env_list(char ***set_new_env)
{
	static char	***env = NULL;

	if (set_new_env)
		env = set_new_env;
	return (env);
}

char	*get_env_value(char **env, char *var_name)
{
	int		i;
	char	*value;
	char	*line_name;

	if (!env)
		env = *(get_env_list(NULL));
	if (env == NULL)
		return (NULL);
	i = -1;
	while (env[++i])
	{
		line_name = get_env_var_name(env[i]);
		if (ft_strcmp(line_name, var_name) == 0)
		{
			free(line_name);
			break ;
		}
	}
	value = ft_substr(env[i], ft_strlen(var_name) + 1, ft_strlen(env[i])
			- ft_strlen(var_name) - 1);
	if (value == NULL)
		return (NULL);
	return (value);
}
