/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 00:47:03 by frapp             #+#    #+#             */
/*   Updated: 2024/01/26 23:03:25 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	size1;
	size_t	size2;
	char	*new_str;

	size1 = ft_strlen(s1);
	size2 = ft_strlen(s2);
	new_str = (char *) ft_calloc((size1 + size2 + 1), sizeof(char));
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, s1, size1 + 1);
	ft_strlcpy(new_str + size1, s2, size2 + 1);
	return (new_str);
}

void	ft_strjoin_inplace(char **s1, char const *s2)
{
	size_t	size1;
	size_t	size2;
	char	*new_str;

	size1 = 0;
	size2 = 0;
	if (*s1)
		size1 = ft_strlen(*s1);
	if (s2)
		size2 = ft_strlen(s2);
	new_str = (char *) ft_calloc((size1 + size2 + 1), sizeof(char));
	if (!new_str)
	{
		free(*s1);
		*s1 = NULL;
		return ;
	}
	ft_strlcpy(new_str, *s1, size1 + 1);
	ft_strlcpy(new_str + size1, s2, size2 + 1);
	if (*s1)
		free(*s1);
	*s1 = new_str;
}

void	ft_strjoin_inplace_char(char **s1, char const s2)
{
	size_t	size1;
	size_t	size2;
	char	*new_str;

	size1 = 0;
	size2 = 1;
	if (*s1)
		size1 = ft_strlen(*s1);
	new_str = (char *) ft_calloc((size1 + size2 + 1), sizeof(char));
	if (!new_str)
	{
		free(*s1);
		*s1 = NULL;
		return ;
	}
	ft_strlcpy(new_str, *s1, size1 + 1);
	new_str[size1] = s2;
	new_str[size1 + 1] = 0;
	if (*s1)
		free(*s1);
	*s1 = new_str;
}
