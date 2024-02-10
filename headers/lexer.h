/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:19:26 by frapp             #+#    #+#             */
/*   Updated: 2024/02/10 20:25:50 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"
# include "tokens.h"

typedef enum e_type				t_type;

typedef struct s_lexer
{
	char	*str;
	int		position;
	int		read_position;
	char	cur_char;
	bool	syntax_error;
}	t_lexer;

t_lexer			new_lexer(char *str);
void			read_char(t_lexer *lexer);
t_token			*next_new_token(t_lexer *lexer);

#endif