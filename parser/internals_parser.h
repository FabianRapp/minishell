/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internals_parser.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 19:21:02 by frapp             #+#    #+#             */
/*   Updated: 2024/03/09 07:12:00 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNALS_PARSER_H
# define INTERNALS_PARSER_H

// parser_type_commands.c
t_result				type_commands(t_parser *parser);

// list_operations/add_new.c
t_parser				*init_parser(char *str);
t_result				insert_dummy_after(t_parser *parser);
t_result				insert_dummy_here(t_parser *parser);

// list_operations/move_nodes.c
void					move_to_arg(t_parser *parser,
							bool is_terminator(t_type), t_type new_type,
							bool as_must_as_possible);
void					move_commands_infront(t_parser *parser);
void					move_next_to_restname(t_parser *parser,
							t_parser **rest_name);

// list_operations/remove_nodes.c
void					trim_whitespace(t_parser *parser);
void					free_ncircular_parser(t_parser *parser, bool free_tok);
void					free_parser_main(t_parser *parser, bool free_tokens);
void					remove_whitespace(t_parser *parser);

// list_operations/utils1.c
void					jump_to_start(t_parser **parser);
void					remove_parser_node(t_parser **node, bool free_tok);
t_parser				*insert_token(t_parser **parser, t_token *token);
t_parser				*last_parser(t_parser *parser);

// parser_ast/parser_ast_redir.c
t_result				append_redir(t_ast *ast_node, t_parser *args,
							t_redir **cur_redir);
t_result				parser_resovle_here_doc(t_redir *redir);

// parser_ast/parser_ast_utils1.c
t_left_right_parsers	split_parser(t_parser *split_location);
t_parser				*find_highest_operator(t_parser *parser);
t_token_list			*extract_token_list(t_parser *parser, char name_or_arg);

// parser_ast/parser_ast_utils2.c
t_arg					*append_arg(t_parser *parser, t_arg *head_arg);

#endif