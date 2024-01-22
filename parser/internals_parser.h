/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internals_parser.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 19:21:02 by frapp             #+#    #+#             */
/*   Updated: 2024/01/22 20:20:59 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNALS_PARSER_H
# define INTERNALS_PARSER_H

bool		add_token(t_parser **parser, t_token *token);
t_parser	*link_parser(char *str);
t_parser	*init_parser(char *str);
bool		continue_parser(t_parser **parser);
bool		reverse_parser(t_parser **parser);
void		free_token(t_token *token);
int			remove_parser_node(t_parser **node);
bool		insert_token(t_parser **insert_after, t_token *insert_token);
void		update_parser_node(t_parser *parser, t_type new_type, char *new_str_data);

#endif