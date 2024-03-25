/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:19:26 by frapp             #+#    #+#             */
/*   Updated: 2024/03/25 01:37:07 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"
# include "tokens.h"

typedef enum e_type	t_type;

typedef struct s_lexer
{
	char	*str;
	int		position;
	int		read_position;
	char	cur_char;
	char	last_char;
}	t_lexer;

t_lexer			new_lexer(char *str);
void			read_char(t_lexer *lexer);
t_token			*next_new_token(t_lexer *lexer, bool recursive_call);

void			print_error_redir_arg(t_lexer *lexer);
#endif