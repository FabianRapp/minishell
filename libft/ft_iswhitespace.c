/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_iswhitespace.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 06:59:45 by frapp             #+#    #+#             */
/*   Updated: 2024/03/06 08:14:17 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

bool	ft_iswhitespace(char c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

bool	contains_non_white_spcace(char *str)
{
	if (!str)
		return (false);
	while (*str)
	{
		if (!ft_iswhitespace(*str))
			return (true);
		str++;
	}
	return (false);
}
