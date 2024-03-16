/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 21:37:54 by frapp             #+#    #+#             */
/*   Updated: 2024/03/06 08:31:24 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

char	*ft_strdup(const char *s1)
{
	size_t	len;
	char	*cpy;
	size_t	i;

	if (!s1)
		return (NULL);
	len = ft_strlen(s1);
	cpy = ft_calloc(len + 1, sizeof(char));
	if (!cpy)
		return (NULL);
	i = 0;
	while (len--)
	{
		cpy[i] = s1[i];
		i++;
	}
	cpy[i] = '\0';
	return (cpy);
}

char	*ft_strndup(const char *s1, size_t max_size)
{
	size_t	len;
	char	*cpy;
	size_t	i;

	len = 0;
	if (s1)
		len = ft_strnlen(s1, max_size);
	cpy = (char *)ft_calloc(len + 1, sizeof(char));
	if (!cpy)
		return (NULL);
	i = 0;
	while (len--)
	{
		cpy[i] = s1[i];
		i++;
	}
	cpy[i] = '\0';
	return (cpy);
}
