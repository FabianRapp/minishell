/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_fill_wildcard_data.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:22:05 by frapp             #+#    #+#             */
/*   Updated: 2024/02/23 16:23:09 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../expansion.h"


t_result	fill_prefix(char **str, t_wildcard_data *w_data)
{
	int				i;

	i = 0;
	while ((*str)[i] && (*str)[i] != '*')
		i++;
	if (i == 0 || !(*str)[i])
		w_data->prefix = NULL;
	else
	{
		w_data->prefix = ft_strndup((*str), i);
		if (!w_data->prefix)
			return (ERROR);
	}
	return (SUCCESS);
	(*str) += i;
}

// check errno for malloc or open/read error
void	fill_wildcard_data(char *wildcard_str, t_wildcard_data *w_data)
{
	int	i;

	if (fill_prefix(&wildcard_str, w_data) == ERROR)
		return ;
	w_data->sub_str = ft_split(wildcard_str, '*');
	if (errno)
		return ;
	if (wildcard_str[ft_strlen(wildcard_str) - 1] == '*')
		w_data->suffix = NULL;
	else
	{
		i = 0;
		while (w_data->sub_str && (w_data->sub_str)[i])
			i++;
		if (i)
		{
			w_data->suffix = (w_data->sub_str)[i - 1];
			(w_data->sub_str)[i - 1] = NULL;
		}
	}
}

void	clean_wildcard_data(t_wildcard_data *w_data)
{
	my_free((void **)&(w_data->prefix));
	my_free((void **)&(w_data->suffix));
	if (w_data->sub_str)
	{
		free_str_ar(w_data->sub_str);
	}
}

bool	matches_wildcard(char *str, t_wildcard_data *w_data)
{
	int	sub_i;
	int	str_i;

	sub_i = 0;
	str_i = 0;
	if (ft_strncmp(str, w_data->prefix, ft_strlen(w_data->prefix)))
		return (false);
	while (w_data->sub_str && (w_data->sub_str)[sub_i])
	{
		while (str[str_i] && ft_strncmp
			(str, (w_data->sub_str)[sub_i], ft_strlen((w_data->sub_str)[sub_i])))
		{
			str_i++;
		}
		if (!str[str_i])
			return (false);
		sub_i++;
		str_i += ft_strlen((w_data->sub_str)[sub_i]);
	}
	if (ft_strncmp(str, w_data->suffix, ft_strlen(w_data->suffix)))
		return (false);
	str_i += ft_strlen(w_data->suffix);
	if (str[str_i] && w_data->suffix)
		return (false);
	return (true);
}
