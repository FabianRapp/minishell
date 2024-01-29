/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_handle_percent.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 18:36:29 by frapp             #+#    #+#             */
/*   Updated: 2023/12/08 06:53:56 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf_handle_percent(int *written_bytes, int fd)
{
	if (write (fd, "%", 1) == -1)
		return (-1);
	(*written_bytes)++;
	return (0);
}
