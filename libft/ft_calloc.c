/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 21:18:07 by frapp             #+#    #+#             */
/*   Updated: 2024/01/29 00:56:34 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*arr;
	int		i;
	int		bytes;

	bytes = count * size;
	arr = (char *)malloc(bytes);
	if (!arr)
	{
		perror(SHELL_NAME "Error: Memory allocation failed");
		return (NULL);
	}
	i = 0;
	while (i < bytes)
		arr[i++] = 0;
	return ((void *)arr);
}
