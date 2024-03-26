/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_wildcards_utils1.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:51:46 by frapp             #+#    #+#             */
/*   Updated: 2024/03/26 22:33:36 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

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

t_result	trim_identifiers(t_token_list *name, bool set_this_true)
{
	char	*tmp;
	char	**to_edit;

	if (set_this_true)
		to_edit = &(name->token->str_data);
	else
		to_edit = &(name->token->old_data);
	tmp = ft_strstrtrim(*to_edit, "1}{");
	free(*to_edit);
	*to_edit = tmp;
	tmp = ft_strstrtrim(tmp, "2}{");
	free(*to_edit);
	*to_edit = tmp;
	tmp = ft_strstrtrim(tmp, "3}{!");
	free(*to_edit);
	*to_edit = tmp;
	tmp = ft_strstrtrim(tmp, "3}{");
	free(*to_edit);
	*to_edit = tmp;
	if (!*to_edit)
		return (ERROR);
	return (trim_identifiers(name, false));
}
