/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 19:24:46 by frapp             #+#    #+#             */
/*   Updated: 2024/01/18 07:27:13 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	haystack_len;
	size_t	needle_len;

	haystack_len = ft_strlen(haystack);
	needle_len = ft_strlen(needle);
	if (haystack_len < len)
		len = haystack_len;
	if (!needle_len)
		return ((char *)haystack);
	while (len-- >= needle_len)
	{
		if (ft_strncmp(haystack, needle, needle_len))
			haystack++;
		else
			return ((char *)haystack);
	}
	return (0);
}
