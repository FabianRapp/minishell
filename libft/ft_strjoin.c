/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 00:47:03 by frapp             #+#    #+#             */
/*   Updated: 2023/10/08 00:56:09 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//untested
char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	size1;
	size_t	size2;
	char	*new_str;

	size1 = ft_strlen(s1);
	size2 = ft_strlen(s2);
	new_str = (char *) malloc(sizeof(char) * (size1 + size2 + 1));
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, s1, size1 + 1);
	ft_strlcpy(new_str + size1, s2, size2 + 1);
	return (new_str);
}
