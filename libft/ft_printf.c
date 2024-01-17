/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 15:47:23 by frapp             #+#    #+#             */
/*   Updated: 2023/10/30 11:12:25 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	logic_part(va_list *arg, const char **format_str, int *written_bytes)
{
	if (!(*format_str))
		return (-1);
	if (**format_str == 'c')
		return (ft_printf_handle_c(arg, written_bytes));
	else if (**format_str == 's')
		return (ft_printf_handle_s(arg, written_bytes));
	else if (**format_str == 'p')
		return (ft_printf_handle_p(arg, written_bytes));
	else if (**format_str == 'd' || **format_str == 'i')
		return (ft_printf_handle_di(arg, written_bytes));
	else if (**format_str == 'u')
		return (ft_printf_handle_u(arg, written_bytes));
	else if (**format_str == 'x')
		return (ft_printf_handle_x(arg, written_bytes));
	else if (**format_str == 'X')
		return (ft_printf_handle_xx(arg, written_bytes));
	else if (**format_str == '%')
		return (ft_printf_handle_percent(written_bytes));
	*written_bytes = -1;
	return (-1);
}

int	ft_printf(const char *format_str, ...)
{
	va_list		arg;
	int			written_bytes;
	int			len;

	va_start(arg, format_str);
	written_bytes = 0;
	while (*format_str && written_bytes >= 0)
	{
		len = 0;
		while (format_str[len] && format_str[len] != '%')
			len++;
		if (write (1, format_str, len) == -1)
			return (-1);
		written_bytes += len;
		format_str += len;
		if (*format_str == '%')
			format_str++;
		if (!(*format_str || written_bytes < 0))
			break ;
		if (logic_part(&arg, &format_str, &written_bytes) == -1)
			break ;
		format_str++;
	}
	va_end(arg);
	return (written_bytes);
}
