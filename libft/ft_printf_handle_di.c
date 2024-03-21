/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_handle_di.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 18:35:25 by frapp             #+#    #+#             */
/*   Updated: 2024/03/21 13:33:12 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../headers/libft.h"

static char	less_zero(int *n)
{
	char	c;

	c = '0' - ((*n) % 10);
	*n /= 10;
	*n = -(*n);
	return (c);
}

static void	loop(int *n, int *i, char *result)
{
	while (*n)
	{
		if (*n < 0)
		{
			result[(*i)--] = less_zero(n);
			continue ;
		}
		result[(*i)--] = '0' + ((*n) % 10);
		(*n) /= 10;
	}
}

int	ft_printf_handle_di(va_list *arg, int *written_bytes, int fd)
{
	char	result[20];
	int		i;
	char	sign;
	int		n;

	n = va_arg(*arg, int);
	sign = 0;
	if (n < 0)
		sign = '-';
	i = 19;
	result[i--] = 0;
	if (!n)
		result[i--] = '0';
	loop(&n, &i, result);
	if (sign)
		result[i--] = '-';
	i++;
	if (write (fd, result + i, ft_strlen(result + i)) == -1)
		return (-1);
	*written_bytes += ft_strlen(result + i);
	return (0);
}
