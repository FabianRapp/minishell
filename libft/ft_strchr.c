/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 23:56:10 by frapp             #+#    #+#             */
/*   Updated: 2024/01/27 21:08:39 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s != (char)c && *s)
		s++;
	if (*s == (char)c)
		return ((char *)s);
	return (0);
}

char	*ft_strchr_fn(const char *s, bool fn(char))
{
	while (*s && !fn(*s))
		s++;
	if (fn(*s))
		return ((char *)s);
	return (0);
}
