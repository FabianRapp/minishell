/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internals.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:21:49 by frapp             #+#    #+#             */
/*   Updated: 2024/02/23 17:34:41 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/lexer.h"

//types
//t_result		wildcard_type(t_lexer *lexer, t_token *token);
t_result		dollar_lexing(t_lexer *lexer, t_token *token);
void			basic_sign_type(t_lexer *lexer, t_token *token);
t_result		literal_type(t_lexer *lexer, t_token *token);
t_result		interpreted_type(t_lexer *lexer, t_token *token);
t_result		redir_type(t_lexer *lexer, t_token *token);
t_result		subshell_type(t_lexer *lexer, t_token *token);
t_result		literal_type2(t_lexer *lexer, t_token *token);
int				name_len(char *str);
