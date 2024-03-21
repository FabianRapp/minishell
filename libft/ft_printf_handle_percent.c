/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_handle_percent.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 18:36:29 by frapp             #+#    #+#             */
/*   Updated: 2024/03/21 13:33:23 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../headers/libft.h"

int	ft_printf_handle_percent(int *written_bytes, int fd)
{
	if (write (fd, "%", 1) == -1)
		return (-1);
	(*written_bytes)++;
	return (0);
}
