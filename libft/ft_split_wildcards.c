/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_wildcards.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 01:18:40 by frapp             #+#    #+#             */
/*   Updated: 2024/03/21 16:00:01 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

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

static void	loop_while_the_four(int *i, char const *s)
{
	while (s[*i] == '2' && s[(*i) + 1] == '}' && s[(*i) + 2] == '{'
		&& s[(*i) + 3] == '*')
	{
		*i += 4;
	}
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
		loop_while_the_four(&i, s);
		s_increase = append_next_sub_wildcards(str_ar + i2, s + i);
		if (s_increase == -1)
			return (ft_free_2darr(str_ar), NULL);
		if (s_increase)
		{
			ft_strstrtrim(str_ar[i2++], "}{");
			i += s_increase;
		}
	}
	return (str_ar);
}
