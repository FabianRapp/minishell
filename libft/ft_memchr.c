/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 17:55:32 by frapp             #+#    #+#             */
/*   Updated: 2023/10/08 00:07:19 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libc.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*str;
	unsigned char		ch;

	ch = (unsigned char)c;
	str = (const unsigned char *)s;
	while (n--)
	{
		if (*str == ch)
			return ((void *)str);
		str++;
	}
	return (0);
}
