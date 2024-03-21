/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_handle_c.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 18:35:22 by frapp             #+#    #+#             */
/*   Updated: 2024/03/21 15:16:54 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

int	ft_printf_handle_c(va_list *arg, int *written_bytes, int fd)
{
	char	c;

	c = (char)va_arg(*(arg), int);
	if (write(fd, &c, 1) == -1)
		return (-1);
	(*written_bytes)++;
	return (0);
}
