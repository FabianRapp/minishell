/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_fill_wildcard_data.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:22:05 by frapp             #+#    #+#             */
/*   Updated: 2024/03/18 02:49:40 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../expansion.h"

// debugging
void	print_wildcard_data(t_wildcard_parameters *data)
{
	int	i = 0;
	printf("pre: %s\n", data->prefix);
	printf("mid:\n");
	while (data->sub_str[i])
		printf("%s\n", data->sub_str[i++]);
	printf("su: %s\n", data->suffix);
}

t_result	fill_prefix(char **str, t_wildcard_parameters *w_para)
{
	int		i;

	i = 0;
	w_para->prefix = NULL;
	if (ft_strnstr(*str, "}{*", ft_strlen(*str)) == *str || !ft_strnstr(*str, "}{*", ft_strlen(*str)))
		return (SUCCESS);
	
	w_para->prefix = ft_strndup((*str), ft_strnstr(*str, "}{*", ft_strlen(*str)) - *str);
	if (!w_para->prefix)
		return (ERROR);
	(*str) = ft_strnstr(*str, "}{*", ft_strlen(*str));
	return (SUCCESS);
}

// check errno for malloc or open/read error
t_result	fill_wildcard_data(char *wildcard_str,
	t_wildcard_parameters *w_para)
{
	int	i;

	if (fill_prefix(&wildcard_str, w_para) == ERROR)
		return (ERROR);
	w_para->sub_str = ft_split_wildcards(wildcard_str);
	if (!(w_para->sub_str))
		return (ERROR);
	w_para->suffix = NULL;
	if (!(ft_strnstr(wildcard_str, "}{*", ft_strlen(wildcard_str)) == wildcard_str + ft_strlen(wildcard_str) - 3))
	// if (wildcard_str[ft_strlen(wildcard_str) - 1] == '*')
	// 	w_para->suffix = NULL;
	{
		i = 0;
		while (w_para->sub_str && (w_para->sub_str)[i])
			i++;
		if (i)
		{
			w_para->suffix = (w_para->sub_str)[i - 1];
			(w_para->sub_str)[i - 1] = NULL;
		}
	}
	//print_wildcard_data(w_para);
	return (SUCCESS);
}

bool	matches_wildcard(char *str, t_wildcard_parameters *w_para)
{
	int	sub_i;

	sub_i = 0;
	if (ft_strncmp(str, w_para->prefix, ft_strlen(w_para->prefix)))
		return (false);
	while (w_para->sub_str && (w_para->sub_str)[sub_i])
	{
		while (*str && ft_strncmp(str, (w_para->sub_str)[sub_i],
			ft_strlen((w_para->sub_str)[sub_i])))
		{
			str++;
		}
		if (!*str)
			return (false);
		str += ft_strlen((w_para->sub_str)[sub_i]);
		sub_i++;
	}
	while (ft_strlen(str) > ft_strlen(w_para->suffix))
		str++;
	if (ft_strncmp(str, w_para->suffix, ft_strlen(w_para->suffix)))
		return (false);
	str += ft_strlen(w_para->suffix);
	if (*str && w_para->suffix)
		return (false);
	return (true);
}
