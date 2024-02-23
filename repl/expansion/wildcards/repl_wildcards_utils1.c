/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_wildcards_utils1.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:51:46 by frapp             #+#    #+#             */
/*   Updated: 2024/02/23 19:32:39 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../expansion.h"

char	*next_file_name(DIR *dir)
{
	struct 	dirent	*file;

	file = readdir(dir);
	while (file && (!ft_strcmp(file->d_name, ".") || !ft_strcmp(file->d_name, "..")))
		file = readdir(dir);
	if (!file)
		return (NULL);
	return (ft_strdup(file->d_name));
}

void	clean_wildcard_data(t_wildcard_data *w_data)
{
	my_free((void **)&(w_data->prefix));
	my_free((void **)&(w_data->suffix));
	if (w_data->sub_str)
	{
		free_str_ar(w_data->sub_str);
	}
}
