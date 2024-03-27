/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:24:35 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/27 08:06:47 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

/******************************************************************************/
/* *********************              PARSER:              ****************** */
/******************************************************************************/

typedef enum e_type						t_type;
typedef struct s_token_list				t_token_list;
typedef struct s_arg					t_arg;
typedef struct s_token					t_token;
typedef enum e_result					t_result;
typedef struct s_ast					t_ast;
typedef struct s_redir					t_redir;

typedef struct s_arg
{
	t_type			type;
	t_token_list	*name;
	struct s_arg	*next;
}	t_arg;

typedef struct s_parser					t_parser;
typedef struct s_parser
{
	t_token		*token;
	t_type		p_type;
	t_parser	*next;
	t_parser	*arg;
	t_parser	*rest_name;
}	t_parser;

typedef struct s_token_list
{
	int					arr[1000];
	struct s_token_list	*next;
	t_token				*token;
}	t_token_list;

typedef struct s_left_right_parsers
{
	t_parser	*left;
	t_parser	*right;
}	t_left_right_parsers;

typedef struct s_here_doc_child_data
{
	char	*termination;
	int		fd;
	bool	expand_vars;
	char	*line;
	bool	start;
	int		count;
}	t_here_doc_child_data;

//from directory list_operations:
t_parser				*init_parser(char *str);
t_result				insert_dummy_here(t_parser *parser);
void					move_next_to_restname(t_parser *parser,
							t_parser **rest_name);
void					move_commands_infront(t_parser *parser);
void					move_to_arg(t_parser *parser, bool is_terminator(
								t_type), t_type new_type, bool as_much);
void					remove_whitespace(t_parser *parser);
void					free_parser_main(t_parser *parser, bool free_tokens);
void					free_ncircular_parser(t_parser *parser, bool free_tok);
void					trim_whitespace(t_parser *parser);
t_parser				*last_parser(t_parser *parser);
t_parser				*insert_token(t_parser **parser, t_token *token);
void					remove_parser_node(t_parser **node, bool free_tok);
void					jump_to_start(t_parser **parser);

// From directory parser_ast:
t_result				parser_resovle_here_doc(t_redir *redir);
t_result				append_redir(t_ast *ast_node, t_parser *args,
							t_redir **cur_redir);
t_result				parser_resovle_here_str(t_redir *redir);
t_token_list			*extract_token_list(t_parser *parser, char name_or_arg);
t_left_right_parsers	split_parser(t_parser *split_location);
t_parser				*find_highest_operator(t_parser *parser);
t_arg					*append_arg(t_parser *parser, t_arg *head_arg);
char					*parser_expand_line(char *line);
t_ast					*build_ast(t_parser *parser);
void					init_here_doc_child(int pipe_fd[2], char *termination,
							t_redir *redir);
t_here_doc_child_data	*heredoc_chil_data_state(
							t_here_doc_child_data *new_state);

// rest in "parser" directory:
t_ast					*parser(char *str);
t_result				type_commands(t_parser *parser);
t_result				parse_redir_paths(t_parser *parser);
void					type_args(t_parser *parser);
t_result				has_content(t_parser *parser);
t_parser				*has_none_redir_arg(t_parser *parser);
void					type_command(t_parser *parser, bool *found_command);

#endif	//PARSER_H
