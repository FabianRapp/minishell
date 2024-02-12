/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internals_parser.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 19:21:02 by frapp             #+#    #+#             */
/*   Updated: 2024/02/11 21:08:24 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNALS_PARSER_H
# define INTERNALS_PARSER_H

// list_operations/add_new.c
t_parser	*init_parser(char *str);
bool		insert_dummy(t_parser *parser);

// list_operations/move_nodes.c
void		move_to_arg(t_parser *parser,
			bool is_terminator(t_type), t_type new_type);
void		swap_parsers(t_parser *node1, t_parser *node2);
bool		move_commands_infront(t_parser *parser);
void		move_next_to_name(t_parser *parser, t_parser **rest_name);

// list_operations/remove_nodes.c
void		trim_whitespace(t_parser *parser);
void		free_ncircular_parser(t_parser *parser, bool free_tok);
void		free_parser_main(t_parser *parser, bool free_tokens);
void		remove_whitespace(t_parser *parser);

// groups.c
bool		is_redir(t_type type);
bool		is_redir_arg_terminator(t_type type);
bool		is_word_terminator(t_type type);
bool		command_terminator(t_type type);

// list_operations/utils1.c
void		jump_to_start(t_parser **parser);
void		remove_parser_node(t_parser **node, bool free_tok);
t_parser	*insert_token(t_parser **parser, t_token *token);
t_parser	*last_parser(t_parser *parser);

#endif