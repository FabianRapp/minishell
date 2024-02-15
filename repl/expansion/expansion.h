/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:00:52 by frapp             #+#    #+#             */
/*   Updated: 2024/02/15 05:18:22 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "../../headers/minishell.h"

// word_splitting.c
t_token_list	*word_splitting(t_token_list *list);

// utils1.c
bool			env_to_word_token(t_token *token);
bool			pidreq_to_literal_token(t_env *env, t_token *token);
char			*env_var_to_str(char *env_var);
bool			move_excess_name_to_arg(t_ast *ast);

// utils2.c
bool			expand_interpreted(t_token *token, t_env *env);
char			*expand_dollar(char *dollar_str, t_env *env, int *index);
t_token_list	*remove_non_literals(t_token_list *list);

// utils3.c
char			*get_last_exit_str(void);

#endif