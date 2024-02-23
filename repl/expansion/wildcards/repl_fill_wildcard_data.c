/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_fill_wildcard_data.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 19:22:05 by frapp             #+#    #+#             */
/*   Updated: 2024/02/23 18:05:35 by frapp            ###   ########.fr       */
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
	(*str) += i;
	return (SUCCESS);
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

void print_wildcard_data(const t_wildcard_data *data)
{
	if (data == NULL) {
		printf("The data is NULL.\n");
		return;
	}
	printf("Prefix: %s\n", data->prefix ? data->prefix : "NULL");
	if (data->sub_str == NULL || data->sub_str[0] == NULL)
	{
		printf("no sub strs\n");
	}
	else
	{
		int	i = 0;
		while ((data->sub_str)[i])
		{
			printf("sub_str %d: %s\n", i +1, (data->sub_str)[i]);
			i++;
		}
	}
	printf("Suffix: %s\n", data->suffix ? data->suffix : "NULL");
}



#define MATCHES_WILDCARD_DEBUG 0
bool	matches_wildcard(char *str, t_wildcard_data *w_data)
{
	int	sub_i;

	char	*debug = str;

	// static bool was_here = false;
	// if (!was_here)
	// {
	// 	was_here = true;
	// 	print_wildcard_data(w_data);
	// }
	sub_i = 0;
	if (ft_strncmp(str, w_data->prefix, ft_strlen(w_data->prefix)))
	{
		if (MATCHES_WILDCARD_DEBUG) printf("prefix dosnt match: %s\n", debug);
		return (false);
	}
	while (w_data->sub_str && (w_data->sub_str)[sub_i])
	{
		while (*str && ft_strncmp
			(str, (w_data->sub_str)[sub_i], ft_strlen((w_data->sub_str)[sub_i])))
		{
			str++;
		}
		if (!*str)
		{
			if (MATCHES_WILDCARD_DEBUG) printf("str over with subs left: %s\n", debug);
			return (false);
		}
		str += ft_strlen((w_data->sub_str)[sub_i]);
		sub_i++;
	}
	while (ft_strlen(str) > ft_strlen(w_data->suffix))
		str++;
	if (ft_strncmp(str, w_data->suffix, ft_strlen(w_data->suffix)))
	{
		if (MATCHES_WILDCARD_DEBUG) printf("suffix dosnt match: %s (%s)\n", debug, str);;
		return (false);
	}
	str += ft_strlen(w_data->suffix);
	if (*str && w_data->suffix)
	{
		if (MATCHES_WILDCARD_DEBUG) printf("suffix not found: %s\n", debug);
		return (false);
	}
	if (MATCHES_WILDCARD_DEBUG) printf("str matches: %s\n", debug);
	return (true);
}
