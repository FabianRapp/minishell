/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_wildcards_utils1.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:51:46 by frapp             #+#    #+#             */
/*   Updated: 2024/03/20 02:32:12 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../expansion.h"

char	*next_file_name(DIR *dir, bool hidden)
{
	struct dirent	*file;

	file = readdir(dir);
	while (!hidden && file && (!ft_strncmp(file->d_name, ".", 1)))
		file = readdir(dir);
	while (hidden && file && (!ft_strcmp(file->d_name, ".")
		|| !ft_strcmp(file->d_name, "..")))
		file = readdir(dir);
	if (!file)
		return (NULL);
	return (ft_strdup(file->d_name));
}

void	clean_wildcard_data(t_wildcard_parameters *w_para)
{
	ft_free((void **)&(w_para->prefix));
	ft_free((void **)&(w_para->suffix));
	if (w_para->sub_str)
	{
		ft_free_2darr(w_para->sub_str);
	}
}
