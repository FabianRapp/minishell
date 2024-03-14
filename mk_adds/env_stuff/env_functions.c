/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:36:06 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/14 03:13:15 by mevangel         ###   ########.fr       */
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
	while (line[len] != '=' && line[len] != '\0')
		len++;
	var_name = ft_substr(line, 0, len);
	return (var_name);
}

char	**add_env_var(char *str_to_add, char ***arr_ptr)
{
	char	**env_before;
	int		i;
	char	**new;

	i = 0;
	env_before = *(arr_ptr);
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

	env_before = *(arr_ptr);
	i = 0;
	// while (env_before[i] && !ft_strnstr(env_before[i], var_to_rm, ft_strlen(env_before[i])))
	// 	i++;
	while (env_before[i] && ft_strncmp(var_to_rm, get_env_var_name(env_before[i]), ft_strlen(var_to_rm)))
		i++;
	if (env_before[i] == NULL)
		return (env_before);
	//so now the i is the index of the line of the 2darray that i want to delete:
	new = (char **)malloc(ft_strarr_size(env_before) * sizeof(char *));
	if (new == NULL)
		return (NULL);
	// new[ft_strarr_size(env_before) - 2] = NULL;
	del_index = i;
	i = -1;
	while (++i < del_index)
		new[i] = ft_strdup(env_before[i]);
	// i++; //to pass the del_index of the deleted line
	while (env_before[++i])
		new[i - 1] = ft_strdup(env_before[i]);
	new[i - 1] = NULL;
	ft_free_2darr(env_before);
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

//TODO: should i modify the SHLVL environmental variable? It is always printed as 1, although it should start as 