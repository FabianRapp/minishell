/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internals.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:21:49 by frapp             #+#    #+#             */
/*   Updated: 2024/01/18 11:36:10 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/lexer.h"

//types
bool			env_var_type(t_lexer *lexer, t_token *token);
bool			nv_var_type(t_lexer *lexer, t_token *token);
bool			basic_sign_type(t_lexer *lexer, t_token *token);
bool			literal_type(t_lexer *lexer, t_token *token);
bool			interpreted_type(t_lexer *lexer, t_token *token);
bool			ft_buildin_type(t_lexer *lexer, t_token *token);
bool			redir_type(t_lexer *lexer, t_token *token);
bool			subshell_type(t_lexer *lexer, t_token *token);
bool			flag_type(t_lexer *lexer, t_token *token);
bool			word_type(t_lexer *lexer, t_token *token);


void			read_char(t_lexer *lexer);
t_token			next_new_token(t_lexer *lexer);
void			circ_lst_add_back(t_token **last, t_token **first, t_token *cur);
bool			is_termination(char c);
int				name_len(char *str);

