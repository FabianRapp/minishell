/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 00:57:06 by frapp             #+#    #+#             */
/*   Updated: 2024/03/26 22:34:10 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

// void	expandlist_convert_white_space(t_token *token)
// {
// 	int	i;

// 	if (token->type != WORD)
// 		return ;
// 	if (!token->str_data)
// 		return ;
// 	i = 0;
// 	while (token->str_data[i])
// 	{
// 		if (!ft_iswhitespace(token->str_data[i]))
// 			return ;
// 	}
// 	ft_free((void **)&(token->str_data));
// 	token->type = WHITE_SPACE;
// }

void	init_expand_args(t_arg **cur, t_arg **base_arg, t_arg **last)
{
	*cur = *base_arg;
	*last = NULL;
}
