/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:19:26 by frapp             #+#    #+#             */
/*   Updated: 2024/01/18 13:33:46 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"
# include "tokens.h"

typedef	enum e_redir			t_redir;
typedef enum e_type				t_type;

typedef struct s_lexer
{
	char	*str;
	int		position;
	int		read_position;
	char	cur_char;
}	t_lexer;

t_lexer			new_lexer(char *str);
t_token			*token_list(char *str);
int				remove_token(t_token *token);
bool			list_right(t_token **token);
bool			list_left(t_token **token);
void			skip_whitespace(t_token **cur);


//debug lexer
void			print_token(t_token token);
bool			test_lexer_manualy(char *str);
void			print_list(t_token *first);

#endif