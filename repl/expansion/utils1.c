/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:00:00 by frapp             #+#    #+#             */
/*   Updated: 2024/02/23 21:25:25 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

t_result	env_to_word_token(t_token *token)
{
	char	*env_var;

	token->type = WORD;
	free(token->str_data);
	env_var = getenv(token->old_data);
	if (!env_var)
		token->str_data = ft_strdup("");
	else
		token->str_data = ft_strdup(env_var);
	if (!token->str_data)
		return (ERROR);
	return (SUCCESS);
}

t_result	pidreq_to_literal_token(t_env *env, t_token *token)
{
	token->type = LITERAL;
	token->str_data = ft_itoa(env->main_pid);
	if (!token->str_data)
		return (ERROR);
	return (SUCCESS);
}
