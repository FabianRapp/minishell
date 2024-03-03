/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_wildcards_utils1.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:51:46 by frapp             #+#    #+#             */
/*   Updated: 2024/03/03 22:39:03 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../expansion.h"

char	*next_file_name(DIR *dir)
{
	struct dirent	*file;

	file = readdir(dir);
	while (file && (!ft_strcmp(file->d_name, ".")
			|| !ft_strcmp(file->d_name, "..")))
		file = readdir(dir);
	if (!file)
		return (NULL);
	return (ft_strdup(file->d_name));
}

void	clean_wildcard_data(t_wildcard_parameters *w_para)
{
	my_free((void **)&(w_para->prefix));
	my_free((void **)&(w_para->suffix));
	if (w_para->sub_str)
	{
		free_str_ar(w_para->sub_str);
	}
}
