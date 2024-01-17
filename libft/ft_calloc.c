/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 21:18:07 by frapp             #+#    #+#             */
/*   Updated: 2023/10/08 00:07:25 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*arr;
	int		i;
	int		bytes;

	bytes = count * size;
	arr = (char *)malloc(bytes);
	if (!arr)
		return (NULL);
	i = 0;
	while (i < bytes)
		arr[i++] = 0;
	return ((void *)arr);
}
