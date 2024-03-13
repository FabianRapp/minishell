/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:36:06 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/13 20:14:38 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
env (alone): will show a list of environment varaibles in  the format:
			VARIABLE=value

but it can also be used like:
env VARIABLE=value some_command
		|-> in this case, it runs the command in the modified environment (tmporary)
			where the environmental variable VARIABLE is added

*/

#include "../../headers/minishell.h"
#include "../../headers/eval.h"

// 1. function to return only the env var name.
// 3. function to change the value of an environmental variable
// 4. 

char	*get_env_value(t_ast *ast, char *var_name)
{
	char	**env;
	int		i;
	char	*value;

	env = *(ast->envs);
	i = 0;
	while(env[i] && ft_strncmp(env[i], var_name, ft_strlen(var_name)) != 0)
		i++;
	if (env[i] == NULL) //means the var_name doesn't exist in env
		return (NULL);
	value = ft_substr(env[i], ft_strlen(var_name) + 1, ft_strlen(env[i])
		- ft_strlen(var_name) - 1);
	if (value == NULL)
		return (NULL);
	return (value);
}

char	*get_env_var_name(char *line)
{
	int		len;
	char	*var_name;
	
	len = 0;
	while (line[len] != '=')
		len++;
	var_name = ft_substr(line, 0, len);
	return (var_name);
}

// USER=mevangel

char	**add_env_var(t_ast *ast, char *str_to_add)
{
	char	**env_before;
	int		i;
	char	**new;

	env_before = *(ast->envs);
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
	// free(str_to_add);
	return (new);	
}

char	**ft_initialize_our_env(char **base_env)
{
	int		i;
	char	**ret;

	i = 0;
	while (base_env[i])
		i++;
	ret = malloc((i + 1) * sizeof(char *));
	if (ret == NULL)
		return (NULL);
	ret[i] = NULL;
	i = -1;
	while (base_env[++i])
		ret[i] = ft_strdup(base_env[i]);
	return (ret);
}

// ft_substr()

// TEST=hello