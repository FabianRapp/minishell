/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_handle_xx.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 18:42:50 by frapp             #+#    #+#             */
/*   Updated: 2023/12/08 06:54:53 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf_handle_xx(va_list *arg, int *written_bytes, int fd)
{
	char			digits[17];
	unsigned int	argument;
	char			buffer[12];
	int				i;

	ft_strlcpy(digits, "0123456789ABCDEF", 17);
	argument = va_arg(*arg, unsigned);
	if (!argument)
	{
		if (write(fd, "0", 1) == -1)
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
	if (write(fd, buffer + i, 11 - i) == -1)
		return (-1);
	(*written_bytes) += 11 - i;
	return (0);
}
