/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 17:12:31 by frapp             #+#    #+#             */
/*   Updated: 2024/01/18 09:01:14 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

char	*cleanup_libft(t_file *current_file, t_file *first_file)
{
	t_file		*temp;

	temp = first_file;
	while (temp && (temp != current_file))
		temp = temp->next_file;
	if (!temp)
		return (NULL);
	current_file->first_file = NULL;
	current_file->fd = 0;
	if (current_file != first_file && current_file)
	{
		temp = first_file;
		while ((temp->next_file) != current_file)
			temp = temp->next_file;
		temp->next_file = current_file->next_file;
		free(current_file);
	}
	return (NULL);
}

t_file	*get_current_file(int fd, t_file *first_file)
{
	t_file	*current_file;

	current_file = first_file;
	while ((current_file->fd != fd) && (current_file->next_file))
		current_file = current_file->next_file;
	if ((current_file->fd) != fd)
	{
		current_file->next_file = (t_file *)malloc(sizeof(t_file));
		if (!current_file->next_file)
			return (NULL);
		current_file = current_file->next_file;
		current_file->first_file = first_file;
		current_file->next_file = NULL;
		current_file->fd = fd;
		current_file->buffer1_chars = 0;
		current_file->buffer1_i = 0;
	}
	(current_file->return_now) = 0;
	current_file->cur_all_c = 1;
	current_file->output = NULL;
	current_file->output_size = 0;
	return (current_file);
}

int	my_str_cpy(char *dest, char *src, char *src2)
{
	int	len;
	int	i2;

	i2 = 0;
	len = 0;
	while (dest[len])
		len++;
	if (src)
	{
		while (src[i2])
			dest[len++] = src[i2++];
	}
	if (src2)
	{
		i2 = 0;
		while (src2[i2])
			dest[len++] = src2[i2++];
	}
	dest[len] = 0;
	return (len);
}

void	sort_buffer(t_file *file)
{
	int		i2;

	i2 = 0;
	while ((file->buffer)[file->buffer1_i] && (file->return_now) != 1)
	{
		(file->buffer1_chars)--;
		(file->output_size)++;
		(file->buffer_copy)[i2] = (file->buffer)[file->buffer1_i];
		(file->buffer)[((file->buffer1_i)++)] = 0;
		if ((file->buffer_copy)[i2++] == '\n')
			(file->return_now) = 1;
	}
	(file->buffer_copy)[i2] = '\0';
}

char	*append_buffer(t_file *current_file)
{
	char	*temp;

	sort_buffer(current_file);
	if ((current_file->cur_all_c) > (current_file->output_size))
		my_str_cpy(current_file->output, current_file->buffer_copy, NULL);
	else
	{
		(current_file->cur_all_c) += (current_file->output_size);
		temp = (char *)malloc(sizeof(char) * (current_file->cur_all_c));
		if (!temp)
		{
			current_file->buffer1_chars = -1;
			free(current_file->output);
			return (NULL);
		}
		*temp = 0;
		my_str_cpy(temp, current_file->output, current_file->buffer_copy);
		free(current_file->output);
		current_file->output = temp;
	}
	return (current_file->output);
}
