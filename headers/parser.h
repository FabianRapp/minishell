/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 08:52:07 by frapp             #+#    #+#             */
/*   Updated: 2024/01/22 16:47:59 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

typedef struct s_directory	t_directory;
typedef struct s_directory
{
	char		*str;
	t_directory	*next;
}	t_directory;
/*
	parser is a circuylar linked list unless its 1 or 0 elements
*/

typedef struct s_command
{
	int			pid_io[2];
} t_command;

typedef struct s_parser	t_parser;
typedef struct s_parser
{
	t_token		*token;
	t_type		type;
	t_type		next_type;
	t_type		last_type;
	t_parser	*last;
	t_parser	*next;
	t_directory	*out;
	t_directory	*in;
}	t_parser;


t_parser	*init_parser(char *str);
bool		add_token(t_parser **parser, t_token *token);
t_parser	*link_parser(char *str);
bool		continue_parser(t_parser **parser);
bool		reverse_parser(t_parser **parser);
void		free_token(t_token *token);
int			remove_parser_node(t_parser **node);
bool		insert_token(t_parser **insert_after, t_token *insert_token);


#endif
