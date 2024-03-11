/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:24:35 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/10 21:16:11 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/eval.h"

int	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX))
	{
		printf("%s\n", cwd);
		return (SUCCESS);
	}
	else
		return (ERROR);
}
