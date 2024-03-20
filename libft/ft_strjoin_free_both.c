/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_free_both.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:02:10 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/20 08:04:36 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

char	*ft_strjoin_free_both(char *s1, char *s2)
{
	char	*joined;
	int		i;
	int		j;

	joined = ft_calloc(ft_strlen(s1) + ft_strlen(s2) + 1, 1);
	if (!joined)
		return (NULL);
	i = -1;
	j = -1;
	while (s1[++i] != '\0')
		joined[i] = s1[i];
	while (s2[++j] != '\0')
		joined[i + j] = s2[j];
	free(s1);
	free(s2);
	return (joined);
}
