/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 03:14:24 by frapp             #+#    #+#             */
/*   Updated: 2023/10/08 03:43:50 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	fill_str(char *str, int n, int size)
{
	str[size--] = '\0';
	if (n == 0)
		str[0] = '0';
	else if (n < 0)
	{
		str[0] = '-';
		str[size--] = '0' - n % 10;
		n /= 10;
		n = -n;
	}
	while (n)
	{
		str[size--] = '0' + n % 10;
		n /= 10;
	}
}

char	*ft_itoa(int n)
{
	int		size;
	int		n_cpy;
	char	*str;

	n_cpy = n;
	if (n <= 0)
		size = 1;
	else
		size = 0;
	while (n)
	{
		n /= 10;
		size++;
	}
	str = (char *)malloc(sizeof(char) * (size + 1));
	if (!str)
		return (NULL);
	fill_str(str, n_cpy, size);
	return (str);
}
