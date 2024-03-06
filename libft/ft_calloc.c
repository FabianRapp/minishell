/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 21:18:07 by frapp             #+#    #+#             */
/*   Updated: 2024/03/06 00:33:20 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"
#include "../headers/minishell.h"

// minishell
static void	print_error_calloc(bool shell_name, char *command_name, char *arg, char *str)
{
	if (shell_name)
		ft_fprintf(2, "%s: ", SHELL_NAME);
	if (command_name)
		ft_fprintf(2, "%s: ", command_name);
	if (arg)
		ft_fprintf(2, "%s: ", arg);
	if (str)
		ft_fprintf(2, "%s", str);
	ft_fprintf(2, "\n");
}

void	*ft_calloc(size_t count, size_t size)
{
	char	*arr;
	int		i;
	int		bytes;

	bytes = count * size;
	arr = (char *)malloc(bytes);
	if (!arr)
	{
		print_error_calloc(true, NULL,"Error", "Memory allocation failed");
		set_last_exit(errno);
		return (NULL);
	}
	i = 0;
	while (i < bytes)
		arr[i++] = 0;
	return ((void *)arr);
}
