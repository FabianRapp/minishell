/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_handle_s.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 18:36:32 by frapp             #+#    #+#             */
/*   Updated: 2024/03/21 15:17:07 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

int	ft_printf_handle_s(va_list *arg, int *written_bytes, int fd)
{
	char	*argument;

	argument = va_arg(*arg, char *);
	if (!argument)
	{
		if (write(fd, "(null)", 6) == -1)
			return (-1);
		(*written_bytes) += 6;
		return (0);
	}
	if (ft_strlen(argument))
	{
		if (write(fd, argument, ft_strlen(argument)) == -1)
			return (-1);
	}
	(*written_bytes) += ft_strlen(argument);
	return (0);
}
