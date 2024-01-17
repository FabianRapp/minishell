/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 01:20:00 by frapp             #+#    #+#             */
/*   Updated: 2023/10/08 00:08:50 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*bytes;
	unsigned char	val;

	bytes = (unsigned char *)b;
	val = (unsigned char) c;
	while (len)
		bytes[len-- - 1] = val;
	return (b);
}
