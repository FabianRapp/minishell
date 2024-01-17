/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_handle_u.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 18:36:35 by frapp             #+#    #+#             */
/*   Updated: 2023/10/18 09:30:12 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf_handle_u(va_list *arg, int *written_bytes)
{
	char			result[20];
	int				i;
	char			sign;
	unsigned int	n;

	n = va_arg(*arg, unsigned);
	sign = 0;
	if (n < 0)
		sign = '-';
	i = 19;
	result[i--] = 0;
	if (!n)
		result[i--] = '0';
	while (n)
	{
		result[i--] = '0' + (n % 10);
		n /= 10;
	}
	if (sign)
		result[i--] = '-';
	i++;
	if (write (1, result + i, ft_strlen(result + i)) == -1)
		return (-1);
	*written_bytes += ft_strlen(result + i);
	return (0);
}
