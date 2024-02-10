/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internals_parser.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 19:21:02 by frapp             #+#    #+#             */
/*   Updated: 2024/02/10 22:21:29 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNALS_PARSER_H
# define INTERNALS_PARSER_H

// list_operations/init_cleanup.c
t_parser	*init_parser(char *str);
void		free_ncircular_parser(t_parser *parser, bool free_tok);
void		free_parser_main(t_parser *parser, bool free_tokens);

// groups.c
bool		is_redir(t_type type);
bool		is_redir_arg_terminator(t_type type);
bool		is_word_terminator(t_type type);
bool		command_terminator(t_type type);

// list_operations/utils1.c
void		jump_to_start(t_parser **parser);
void		remove_parser_node(t_parser **node, bool free_tok);
t_parser	*insert_token(t_parser **parser, t_token *token);


#endif