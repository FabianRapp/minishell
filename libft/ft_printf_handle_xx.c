/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_handle_XX.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 18:42:50 by frapp             #+#    #+#             */
/*   Updated: 2023/10/18 09:30:23 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf_handle_xx(va_list *arg, int *written_bytes)
{
	char			digits[17];
	unsigned int	argument;
	char			buffer[12];
	int				i;

	ft_strlcpy(digits, "0123456789ABCDEF", 17);
	argument = va_arg(*arg, unsigned);
	if (!argument)
	{
		if (write(1, "0", 1) == -1)
			return (-1);
		(*written_bytes)++;
		return (0);
	}
	i = 11;
	buffer[11] = '\0';
	while (argument)
	{
		buffer[--i] = digits[argument % 16];
		argument /= 16;
	}
	if (write(1, buffer + i, 11 - i) == -1)
		return (-1);
	(*written_bytes) += 11 - i;
	return (0);
}
