/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 16:26:31 by frapp             #+#    #+#             */
/*   Updated: 2024/03/22 02:20:24 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	const unsigned char	*str1;
	const unsigned char	*str2;

	str1 = (const unsigned char *)s1;
	str2 = (const unsigned char *)s2;
	if (!s1 && !s2)
		return (0);
	if (!s1)
		return (-*s2);
	if (!s2)
		return (*s1);
	while (n && *str1 && *str2 && !(*str1 - *str2))
	{
		str1++;
		str2++;
		n--;
	}
	if (n)
		return (*str1 - *str2);
	return (0);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	// if (!s1 && !s2)
	// 	return (0);
	// if (!s1)
	// 	return (-*s2);
	// if (!s2)
	// 	return (*s1);
	while (*s1)
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}
