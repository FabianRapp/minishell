/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_fill_wildcard_data.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:22:05 by frapp             #+#    #+#             */
/*   Updated: 2024/02/25 05:57:54 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../expansion.h"


t_result	fill_prefix(char **str, t_wildcard_parameters *w_para)
{
	int				i;

	i = 0;
	while ((*str)[i] && (*str)[i] != '*')
		i++;
	if (i == 0 || !(*str)[i])
		w_para->prefix = NULL;
	else
	{
		w_para->prefix = ft_strndup((*str), i);
		if (!w_para->prefix)
			return (ERROR);
	}
	(*str) += i;
	return (SUCCESS);
}

// check errno for malloc or open/read error
void	fill_wildcard_data(char *wildcard_str, t_wildcard_parameters *w_para)
{
	int	i;

	if (fill_prefix(&wildcard_str, w_para) == ERROR)
		return ;
	w_para->sub_str = ft_split(wildcard_str, '*');
	if (errno)
		return ;
	if (wildcard_str[ft_strlen(wildcard_str) - 1] == '*')
		w_para->suffix = NULL;
	else
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
}

bool	matches_wildcard(char *str, t_wildcard_parameters *w_para)
{
	int	sub_i;

	sub_i = 0;
	if (ft_strncmp(str, w_para->prefix, ft_strlen(w_para->prefix)))
		return (false);
	while (w_para->sub_str && (w_para->sub_str)[sub_i])
	{
		while (*str && ft_strncmp
			(str, (w_para->sub_str)[sub_i], ft_strlen((w_para->sub_str)[sub_i])))
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
