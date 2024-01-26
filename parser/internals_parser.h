/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internals_parser.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 19:21:02 by frapp             #+#    #+#             */
/*   Updated: 2024/01/26 02:01:43 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNALS_PARSER_H
# define INTERNALS_PARSER_H

bool		add_token_back(t_parser **parser, t_token *token);
t_parser	*link_parser(char *str);
t_parser	*init_parser(char *str);
void		free_token(t_token *token);
bool		insert_token(t_parser **insert_after, t_token *insert_token);

#endif