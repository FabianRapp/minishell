/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 01:18:40 by frapp             #+#    #+#             */
/*   Updated: 2024/03/19 00:20:08 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

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
		while (*s && *s == c)
			s++;
		s_increase = append_next_sub(str_ar + i, s, c);
		if (s_increase == -1)
			return (ft_free_2darr(str_ar), NULL);
		if (s_increase)
			i++;
		s += s_increase;
	}
	return (str_ar);
}

static int	append_next_sub_wildcards(char **sub_str, const char *s)
{
	int		count;
	char	*found;

	found = ft_strnstr(s, "2}{*", ft_strlen(s));
	*sub_str = (char *)ft_calloc(ft_strlen(s)+ 1, sizeof(char));
	if (!(*sub_str))
		return (-1);
	count = 0;
	while (s[count] && s + count != found)
	{
		(*sub_str)[count] = s[count];
		count++;
	}
	return (count);
}

char	**ft_split_wildcards(char const *s)
{
	char		**str_ar;
	int			i;
	int			s_increase;
	int			i2;

	if (!s || !*s)
		return (NULL);
	str_ar = (char **)ft_calloc(ft_strlen(s) + 1, sizeof(char *));
	if (!str_ar)
		return (NULL);
	i = 0;
	i2 = 0;
	while (s[i])
	{
		while (s[i] == '2' && s[i + 1] == '}' && s[i + 2] == '{' && s[i + 3] == '*')
			i += 4;
		s_increase = append_next_sub_wildcards(str_ar + i2, s + i);
		if (s_increase == -1)
			return (ft_free_2darr(str_ar), NULL);
		if (s_increase)
		{
			ft_strstrtrim(str_ar[i2], "}{");
			i2++;
			i += s_increase;
		}
		
	}
	return (str_ar);
}

static size_t	get_str_count_fn(const char *s, bool is_sep(char))
{
	size_t	count;
	int		was_c;

	count = 0;
	was_c = 1;
	while (*s)
	{
		if (!was_c && is_sep(*s))
		{
			count++;
			was_c = 1;
		}
		else if (is_sep(*s))
			was_c = 1;
		else
			was_c = 0;
		s++;
	}
	if (!was_c)
		count++;
	return (count);
}

static int	append_next_sub_fn(char **sub_str, const char *s, bool is_sep(char))
{
	int		count;
	int		len;
	char	*found;

	found = ft_strchr_fn(s, is_sep);
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
	while (s[count] && !is_sep(s[count]))
	{
		(*sub_str)[count] = s[count];
		count++;
	}
	return (count);
}

char	**ft_split_fn(char const *s, bool is_sep(char))
{
	size_t		str_count;
	char		**str_ar;
	int			i;
	int			s_increase;

	str_count = get_str_count_fn(s, is_sep);
	str_ar = (char **)ft_calloc(str_count + 1, sizeof(char *));
	if (!str_ar)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s && is_sep(*s))
			s++;
		s_increase = append_next_sub_fn(str_ar + i, s, is_sep);
		if (s_increase == -1)
			return (ft_free_2darr(str_ar), NULL);
		if (s_increase)
			i++;
		s += s_increase;
	}
	return (str_ar);
}
