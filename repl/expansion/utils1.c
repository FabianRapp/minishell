/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:00:00 by frapp             #+#    #+#             */
/*   Updated: 2024/02/09 19:32:48 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"


bool	env_to_word_token(t_token *token)
{
	char	*old_data;

	old_data = token->str_data;
	token->type = WORD;
	token->str_data = ft_strdup(getenv(old_data));
	my_free((void **) &old_data);
	if (!token->str_data)
		return (false);
	return (true);
}

bool	pidreq_to_literal_token(t_env *env, t_token *token)
{
	token->type = LITERAL;
	token->str_data = ft_itoa(env->main_pid);
	if (!token->str_data)
		return (false);
	return (true);
}

char	*env_var_to_str(char *env_var)
{
	char	*not_free_able;

	if (ft_strlen(env_var) <= 0)
		return (NULL);
	not_free_able = getenv(env_var);
	if (!not_free_able)
		return (NULL);
	if (!*not_free_able)
		return (NULL);
	return (ft_strdup(getenv(env_var)));
}
