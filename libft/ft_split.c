/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 01:18:40 by frapp             #+#    #+#             */
/*   Updated: 2023/10/23 16:07:22 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_str_count(const char *s, char c)
{
	size_t	count;
	int		was_c;

	count = 0;
	was_c = 1;
	while (*s)
	{
		if (!was_c && *s == c)
		{
			count++;
			was_c = 1;
		}
		else if (*s == c)
			was_c = 1;
		else
			was_c = 0;
		s++;
	}
	if (!was_c)
		count++;
	return (count);
}

static int	append_next_sub(char **sub_str, const char *s, char c)
{
	int		count;
	int		len;
	char	*found;

	found = ft_strchr(s, c);
	if (!found)
		len = ft_strlen(s);
	else
		len = found - s;
	if (len == 0)
		return (0);
	*sub_str = (char *)ft_calloc(len + 1, sizeof(char));
	if (!(*sub_str))
		return (-1);
	count = 0;
	while (s[count] && s[count] != c)
	{
		(*sub_str)[count] = s[count];
		count++;
	}
	return (count);
}

static char	**free_str_ar(char **str_arr)
{
	int	i;

	i = 0;
	while (str_arr[i] != 0)
		free(str_arr[i++]);
	free(str_arr);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	size_t		str_count;
	char		**str_ar;
	int			i;
	int			s_increase;

	str_count = get_str_count(s, c);
	str_ar = (char **)ft_calloc(str_count + 1, sizeof(char *));
	if (!str_ar)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		s_increase = append_next_sub(str_ar + i, s, c);
		if (s_increase == -1)
			return (free_str_ar(str_ar));
		if (s_increase)
			i++;
		s += s_increase;
	}
	return (str_ar);
}
