/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_wildcards.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 01:18:40 by frapp             #+#    #+#             */
/*   Updated: 2024/03/24 01:55:08 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

static int	append_next_sub_wildcards(char **sub_str,
	const char *rest_wildcard_str)
{
	int		count;
	char	*found;

	found = ft_strnstr(rest_wildcard_str, "2}{*", ft_strlen(rest_wildcard_str));
	*sub_str = (char *)ft_calloc(ft_strlen(rest_wildcard_str)+ 1, sizeof(char));
	if (!(*sub_str))
		return (-1);
	count = 0;
	while (rest_wildcard_str[count] && rest_wildcard_str + count != found)
	{
		(*sub_str)[count] = rest_wildcard_str[count];
		count++;
	}
	return (count);
}

static void	loop_while_the_four(int *i, char const *s)
{
	while (s[*i] == '2' && s[(*i) + 1] == '}' && s[(*i) + 2] == '{'
		&& s[(*i) + 3] == '*')
	{
		*i += 4;
	}
}

char	**ft_split_wildcards(char const *wildcard_str)
{
	char		**str_ar;
	int			i;
	int			s_increase;
	int			i2;

	if (!wildcard_str || !*wildcard_str)
		return (NULL);
	str_ar = (char **)ft_calloc(ft_strlen(wildcard_str) + 1, sizeof(char *));
	if (!str_ar)
		return (NULL);
	i = 0;
	i2 = 0;
	while (wildcard_str[i])
	{
		loop_while_the_four(&i, wildcard_str);
		s_increase = append_next_sub_wildcards(str_ar + i2, wildcard_str + i);
		if (s_increase == -1)
			return (ft_free_2darr(str_ar), NULL);
		if (s_increase)
		{
			i2++;
			i += s_increase;
		}
	}
	return (str_ar);
}
