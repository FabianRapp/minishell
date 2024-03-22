/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 00:57:46 by frapp             #+#    #+#             */
/*   Updated: 2024/03/21 15:24:24 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

static int	in_set(char const *set, char c)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trim;
	char	*trim_ret;
	int		len;

	if (!s1)
		return (NULL);
	len = ft_strlen(s1);
	while (*s1 && in_set(set, *s1))
	{
		len--;
		s1++;
	}
	while (len > 0 && in_set(set, s1[len - 1]))
		len--;
	trim = (char *)malloc(sizeof(char) * (len + 1));
	if (!trim)
		return (NULL);
	trim_ret = trim;
	trim[len] = '\0';
	while (len-- > 0)
		*(trim++) = *(s1++);
	return (trim_ret);
}

char	*ft_strstrtrim(char const *s1, char const *sub)
{
	char	*trim;

	if (!s1)
		return (NULL);
	trim = NULL;
	while (*s1)
	{
		if (*s1 && !ft_strncmp(s1, sub, ft_strlen(sub)))
		{
			s1 += ft_strlen(sub);
			continue ;
		}
		if (*s1)
			if (!ft_strjoin_inplace_char(&trim, *s1))
				return (NULL);
		s1++;
	}
	return (trim);
}
