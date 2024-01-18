/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 15:09:21 by frapp             #+#    #+#             */
/*   Updated: 2024/01/18 07:24:41 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

// if (!dst && !src && len)
	// 	return (dst);
void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char		*d;
	const char	*s;

	if (!dst && !src && len)
		return (dst);
	if (!len)
		return (dst);
	s = (const char *)src;
	d = (char *) dst;
	if (d < s)
	{
		while (len--)
			*(d++) = *(s++);
		return (dst);
	}
	while (len--)
		*(d + len) = *(s + len);
	return (dst);
}
