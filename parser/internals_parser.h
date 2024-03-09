/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internals_parser.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 19:21:02 by frapp             #+#    #+#             */
/*   Updated: 2024/03/09 03:25:58 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNALS_PARSER_H
# define INTERNALS_PARSER_H

// parser_type_commands.c
t_result	type_commands(t_parser *parser);

// list_operations/add_new.c
t_parser	*init_parser(char *str);
t_result	insert_dummy_after(t_parser *parser);
t_result	insert_dummy_here(t_parser *parser);

// list_operations/move_nodes.c
void		move_to_arg(t_parser *parser,
			bool is_terminator(t_type), t_type new_type, bool as_must_as_possible);
void		move_commands_infront(t_parser *parser);
void		move_next_to_restname(t_parser *parser, t_parser **rest_name);

// list_operations/remove_nodes.c
void		trim_whitespace(t_parser *parser);
void		free_ncircular_parser(t_parser *parser, bool free_tok);
void		free_parser_main(t_parser *parser, bool free_tokens);
void		remove_whitespace(t_parser *parser);

// list_operations/utils1.c
void		jump_to_start(t_parser **parser);
void		remove_parser_node(t_parser **node, bool free_tok);
t_parser	*insert_token(t_parser **parser, t_token *token);
t_parser	*last_parser(t_parser *parser);

#endif