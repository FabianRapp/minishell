/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_fill_wildcard_data.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:22:05 by frapp             #+#    #+#             */
/*   Updated: 2024/03/20 14:32:46 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../expansion.h"

// debugging
void	print_wildcard_data(t_wildcard_parameters *data)
{
	int	i = 0;
	printf("pre: %s\n", data->prefix);
	printf("mid: ");
	if (!data->sub_str || !data->sub_str[i])
		printf("%s\n", NULL);
	else
		printf("\n");
	while (data->sub_str && data->sub_str[i])
		printf("%s\n", data->sub_str[i++]);
	printf("su: %s\n", data->suffix);
}

t_result	fill_prefix(char **str, t_wildcard_parameters *w_para)
{
	int		i;

	//printf("here\n");
	i = 0;
	if (!ft_strnstr(*str, "1}{*", ft_strlen(*str)))
	{
		//printf("no wildcard prefix no error\n");
		return (SUCCESS);
	}
	w_para->prefix = ft_strndup((*str), ft_strnstr(*str, "1}{*", ft_strlen(*str)) - *str);
	if (!w_para->prefix)
	{
		//printf("no wildcard prefix error\n");
		return (ERROR);
	}
	if (ft_strlen(w_para->prefix) == 0)
		ft_free((void **)&(w_para->prefix));
	//printf("prefix after fill_prefix: %s\n", w_para->prefix);
	(*str) = ft_strnstr(*str, "1}{*", ft_strlen(*str));
	*str += 4;
	return (SUCCESS);
}

// check errno for malloc or open/read error
t_result	fill_wildcard_data(char *wildcard_str,
	t_wildcard_parameters *w_para)
{
	w_para->prefix = NULL;
	w_para->sub_str = NULL;
	w_para->suffix = NULL;
	if (fill_prefix(&wildcard_str, w_para) == ERROR)
		return (ERROR);
	if (ft_strnstr(wildcard_str, "3}{*", ft_strlen(wildcard_str)) || ft_strnstr(wildcard_str, "3}{!", ft_strlen(wildcard_str)))
	{
		if (!ft_strnstr(wildcard_str, "3}{*", ft_strlen(wildcard_str)))
			(ft_strnstr(wildcard_str, "3}{!", ft_strlen(wildcard_str)))[3] = '*';
		w_para->suffix = ft_strndup(ft_strnstr(wildcard_str, "3}{*", ft_strlen(wildcard_str)) + 4, ft_strlen(wildcard_str));
		if (!w_para->suffix)
		{
		}
		*ft_strnstr(wildcard_str, "3}{*", ft_strlen(wildcard_str)) = 0;
	} 
	if (ft_strlen(w_para->suffix) == 0)
		ft_free((void **)&(w_para->suffix));
	w_para->sub_str = ft_split_wildcards(wildcard_str);
	if (errno)
		return (ERROR);
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
