/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 17:12:34 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 09:54:30 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

char	*reading(t_file	*file)
{
	while (!(file->return_now))
	{
		if (!(file->buffer1_chars))
		{
			(file->buffer1_chars) = read(file->fd, file->buffer, BUFFER_SIZE);
			file->buffer1_i = 0;
			if ((file->buffer1_chars) < 0)
			{
				free(file->output);
				return (NULL);
			}
			else if (!(file->buffer1_chars))
				return (file->output);
			else if ((file->buffer1_chars) < BUFFER_SIZE)
				(file->return_now) = 2;
			file->buffer[(file->buffer1_chars)] = 0;
		}
		if (!append_buffer(file))
			return (NULL);
	}
	return (file->output);
}

char	*shrink_out(int fd, t_file *first_file)
{
	char	*new_output;
	char	*output;
	t_file	*current_file;
	int		size;

	current_file = get_current_file(fd, first_file);
	if (!current_file)
		return (NULL);
	output = reading(current_file);
	size = current_file->output_size;
	if (current_file->buffer1_chars <= 0)
		cleanup_libft(current_file, first_file);
	if (!output)
		return (NULL);
	new_output = (char *)malloc(sizeof(char) * (size + 1));
	if (!new_output)
	{
		free(output);
		return (cleanup_libft(current_file, first_file));
	}
	*new_output = 0;
	my_str_cpy(new_output, output, NULL);
	free(output);
	return (new_output);
}

static void	cleanup_all(t_file *cur, t_file *first_file)
{
	if (cur->next_file)
		cleanup_all(cur->next_file, first_file);
	cleanup_libft(cur, first_file);
}

char	*get_next_line(int fd, bool cleanup)
{
	static t_file	first_file = {{}, {}, 0, -1, 0, 0, 0, 0, 0, 0, 0};
	t_file			*current_file;

	if (cleanup)
		return (cleanup_all(&first_file, &first_file), NULL);
	if (BUFFER_SIZE <= 0)
		return (NULL);
	if (!(first_file.first_file))
	{
		current_file = &first_file;
		while (current_file && (current_file->fd) != fd)
			current_file = current_file->next_file;
		if (!(current_file))
		{
			first_file.buffer1_i = 0;
			first_file.fd = fd;
			first_file.buffer1_chars = 0;
			first_file.first_file = &first_file;
			first_file.cur_all_c = 1;
		}
	}
	return (shrink_out(fd, &first_file));
}
