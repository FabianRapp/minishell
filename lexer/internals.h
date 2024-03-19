/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internals.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 10:21:49 by frapp             #+#    #+#             */
/*   Updated: 2024/03/19 02:24:45 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNALS_H
# define INTERNALS_H

# include "../headers/lexer.h"

t_result		dollar_lexing(t_lexer *lexer, t_token *token);
void			basic_sign_type(t_lexer *lexer, t_token *token);
t_result		literal_type(t_lexer *lexer, t_token *token);
t_result		interpreted_type(t_lexer *lexer, t_token *token);
t_result		redir_type(t_lexer *lexer, t_token *token, bool recursive_call);
t_result		subshell_type(t_lexer *lexer, t_token *token);
t_result		ident_wildcard_literals(t_lexer *lexer, t_token *token, bool skip_next_term);
int				name_len(char *str);
void			lexer_error(t_token *token);
bool			is_redir_terminator_char(char c);
char			*check_limis_potential_fd(char *left_redir_arg,
					t_lexer *lexer, t_lexer lexer_backup)
#endif