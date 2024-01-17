/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_handle_c.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 18:35:22 by frapp             #+#    #+#             */
/*   Updated: 2023/10/22 15:45:00 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdarg.h>

int	ft_printf_handle_c(va_list *arg, int *written_bytes)
{
	char	c;

	c = (char)va_arg(*(arg), int);
	if (write(1, &c, 1) == -1)
		return (-1);
	(*written_bytes)++;
	return (0);
}
