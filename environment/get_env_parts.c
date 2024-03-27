/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_parts.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:36:06 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/27 05:21:33 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

char	*get_env_var_name(char *line, bool plus)
{
	int		len;
	char	*var_name;

	len = 0;
	if (!line)
		return (NULL);
	while (line[len] && line[len] != '=' && !(plus && line[len] == '+'))
	{
		if (line[len] == '+' && line[len + 1] == '=')
			break ;
		len++;
	}
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

static char	*handle_buffer(char *value, char *buffer, int buf_size)
{
	if (!buffer || (int)ft_strlen(value) >= buf_size)
	{
		if (buffer)
		{
			free(value);
			return (NULL);
		}
		return (value);
	}
	ft_strlcpy(buffer, value, buf_size);
	free(value);
	return (buffer);
}

// enters the value into the buffer
// unless strlen(value) >= buf_size ||
// buffer == NULL, in that case the value
// is allotcated
// even without allocated return internally allocation
// if the value does not fit in the buffer and a buffer is given NULL is
// retured
// returns the pointer to the value
char	*get_env_value(char **env, char *var_name, char *buffer, int buf_size)
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
		line_name = get_env_var_name(env[i], false);
		if (ft_strcmp(line_name, var_name) == 0)
		{
			free(line_name);
			break ;
		}
		free(line_name);
	}
	value = ft_substr(env[i], ft_strlen(var_name) + 1, ft_strlen(env[i])
			- ft_strlen(var_name) - 1);
	return (handle_buffer(value, buffer, buf_size));
}
