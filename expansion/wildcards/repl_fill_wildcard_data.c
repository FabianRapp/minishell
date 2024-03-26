/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_fill_wildcard_data.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:22:05 by frapp             #+#    #+#             */
/*   Updated: 2024/03/26 04:28:52 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../expansion.h"

void	print_wildcard_data(t_wildcard_parameters *data)
{
	int	i;

	i = 0;
	printf("pre: %s\n", data->prefix);
	printf("mid: ");
	if (!data->sub_str || !data->sub_str[i])
		printf("%s\n", (char *) NULL);
	else
		printf("\n");
	while (data->sub_str && data->sub_str[i])
		printf("%s\n", data->sub_str[i++]);
	printf("su: %s\n", data->suffix);
}

t_result	fill_prefix(char **str, t_wildcard_parameters *w_para)
{
	if (!ft_strnstr(*str, "1}{*", ft_strlen(*str)))
	{
		return (SUCCESS);
	}
	w_para->prefix = ft_strndup((*str), ft_strnstr(*str, "1}{*",
				ft_strlen(*str)) - *str);
	if (!w_para->prefix)
	{
		return (ERROR);
	}
	if (ft_strlen(w_para->prefix) == 0)
		ft_free((void **)&(w_para->prefix));
	(*str) = ft_strnstr(*str, "1}{*", ft_strlen(*str));
	*str += 4;
	return (SUCCESS);
}

t_result	fill_wildcard_data(char *wldcd_str,
	t_wildcard_parameters *w_para)
{
	w_para->prefix = NULL;
	w_para->sub_str = NULL;
	w_para->suffix = NULL;
	if (fill_prefix(&wldcd_str, w_para) == ERROR)
		return (ERROR);
	if (ft_strnstr(wldcd_str, "3}{*", ft_strlen(wldcd_str))
		|| ft_strnstr(wldcd_str, "3}{!", ft_strlen(wldcd_str)))
	{
		if (!ft_strnstr(wldcd_str, "3}{*", ft_strlen(wldcd_str)))
			(ft_strnstr(wldcd_str, "3}{!", ft_strlen(wldcd_str)))[3] = '*';
		w_para->suffix = ft_strndup(ft_strnstr(wldcd_str, "3}{*",
					ft_strlen(wldcd_str)) + 4, ft_strlen(wldcd_str));
		if (!w_para->suffix)
			return (ERROR);
		*ft_strnstr(wldcd_str, "3}{*", ft_strlen(wldcd_str)) = 0;
	}
	if (ft_strlen(w_para->suffix) == 0)
		ft_free((void **)&(w_para->suffix));
	w_para->sub_str = ft_split_wildcards(wldcd_str);
	if (errno)
		return (ERROR);
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
