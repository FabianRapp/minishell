/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 00:43:46 by frapp             #+#    #+#             */
/*   Updated: 2024/03/25 00:44:03 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"
#include "../headers/lexer.h"

void	free_token(t_token *token)
{
	if (!token)
		return ;
	ft_free((void **)&(token->old_data));
	ft_free((void **)&(token->str_data));
	ft_free((void **)&(token->left_redir_arg));
	free(token);
}

bool	ft_free(void **ptr)
{
	if (!ptr)
		return (false);
	if (!*ptr)
		return (false);
	free(*ptr);
	*ptr = NULL;
	return (true);
}

int	ft_close(int *fd)
{
	int	ret;

	ret = 0;
	if (*fd != INIT_VAL)
		ret = close(*fd);
	*fd = INIT_VAL;
	return (ret);
}
