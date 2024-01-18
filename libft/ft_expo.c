/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 04:16:33 by frapp             #+#    #+#             */
/*   Updated: 2024/01/18 07:24:41 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

int	ft_expo(int base, int expo)
{
	int	result;
	int	i;

	i = 0;
	result = 1;
	if (expo >= 0)
	{
		while (i < expo)
		{
			result *= base;
			i++;
		}
	}
	else
	{
		while (i < -expo)
		{
			result *= base;
			i++;
		}
		result = 1 / result;
	}
	return (result);
}
