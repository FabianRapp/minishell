/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:47:19 by frapp             #+#    #+#             */
/*   Updated: 2024/01/17 05:41:27 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <tokens.h>

void	zero_token(t_token *token)
{
	token->type = UNKNOWN;
	//token->char_count = 0;
	token->int_val = 0;
	token->str = NULL;
	token->unknown = 0;
}
