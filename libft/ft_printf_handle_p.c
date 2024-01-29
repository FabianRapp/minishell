/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_handle_p.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 18:36:26 by frapp             #+#    #+#             */
/*   Updated: 2023/12/08 06:56:10 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	handle_0(int *written_bytes, int fd)
{
	if (write (fd, "0x0", 3) == -1)
		return (-1);
	(*written_bytes) += 3;
	return (0);
}

int	ft_printf_handle_p(va_list *arg, int *written_bytes, int fd)
{
	char				digits[17];
	long long unsigned	argument;
	char				buffer[30];
	int					i;

	ft_strlcpy(digits, "0123456789abcdef", 17);
	argument = va_arg(*arg, long long unsigned);
	if (!argument)
		return (handle_0(written_bytes, fd));
	i = 29;
	buffer[i--] = '\0';
	while (argument)
	{
		buffer[i--] = digits[argument % 16];
		argument /= 16;
	}
	buffer[i--] = 'x';
	buffer[i] = '0';
	if (write (fd, buffer + i, ft_strlen(buffer + i)) == -1)
		return (-1);
	(*written_bytes) += ft_strlen(buffer + i);
	return (0);
}
