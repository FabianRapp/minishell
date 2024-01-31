/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internals.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:21:49 by frapp             #+#    #+#             */
/*   Updated: 2024/01/30 02:59:46 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/lexer.h"

//types
bool			env_var_type(t_lexer *lexer, t_token *token, bool *malloc_error);
bool			basic_sign_type(t_lexer *lexer, t_token *token);
bool			literal_type(t_lexer *lexer, t_token *token, bool *malloc_error);
bool			interpreted_type(t_lexer *lexer, t_token *token, bool *malloc_error);
bool			redir_type(t_lexer *lexer, t_token *token);
bool			subshell_type(t_lexer *lexer, t_token *token, bool *malloc_error);
bool			literal_type2(t_lexer *lexer, t_token *token, bool *malloc_error);
int				name_len(char *str);
