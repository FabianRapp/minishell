/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:19:26 by frapp             #+#    #+#             */
/*   Updated: 2024/01/17 03:12:19 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <minishell.h>
# include <tokens.h>


typedef struct s_lexer
{
	char	*str;
	int		position;
	int		read_position;
	char	cur_char;
}	t_lexer;

t_token		next_token(t_lexer *lexer);
t_lexer		new_lexer(char *str);


// utils
void		read_char(t_lexer *lexer);

#endif