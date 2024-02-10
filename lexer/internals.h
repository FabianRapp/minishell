/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internals.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:21:49 by frapp             #+#    #+#             */
/*   Updated: 2024/02/10 21:32:32 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/lexer.h"

//types
bool			dollar_lexing(t_lexer *lexer, t_token *token);
bool			basic_sign_type(t_lexer *lexer, t_token *token);
bool			literal_type(t_lexer *lexer, t_token *token);
bool			interpreted_type(t_lexer *lexer, t_token *token);
bool			redir_type(t_lexer *lexer, t_token *token);
bool			subshell_type(t_lexer *lexer, t_token *token);
bool			literal_type2(t_lexer *lexer, t_token *token);
int				name_len(char *str);
