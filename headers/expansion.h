/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:24:35 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/27 00:08:19 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "minishell.h"

/******************************************************************************/
/* *********************            EXPANSION:             ****************** */
/******************************************************************************/

# ifndef RETURN_NOW
#  define RETURN_NOW 1
# endif

# ifndef CONTINUE
#  define CONTINUE 2
# endif

# ifndef GO_ON
#  define GO_ON 3
# endif

typedef enum e_result			t_result;
typedef struct s_ast			t_ast;
typedef struct s_token_list		t_token_list;
typedef struct s_shared_data	t_shared_data;
typedef struct s_token			t_token;
typedef struct s_arg			t_arg;

t_result		expansion(t_ast *ast);
t_result		expand_args(t_ast *ast, t_arg **base_arg, bool here_doc);
t_result		word_splitting(t_token_list **list);
void			add_arg_front(t_arg **head, t_arg *new_arg);
t_result		env_to_word_token(t_token *token);
t_result		pidreq_to_literal_token(t_shared_data *env, t_token *token);
int				check_empty_arg(t_arg *last, t_arg **cur,
					t_ast *ast, t_arg **base_arg);
t_result		merge_literals(t_token_list *node);
t_result		expand_interpreted(t_token *token);
t_token_list	*move_nodes_ahead(t_token_list *list, bool set_this_true);
t_token_list	*remove_non_literals(t_token_list *list);
t_result		expansion_move_to_arg(t_arg **next_arg, t_token_list *list);
// void	expandlist_convert_white_space(t_token *token); //? not used ?
void			init_expand_args(t_arg **cur, t_arg **base_arg, t_arg **last);

// WILDCARDS:
typedef struct s_wildcard_parameters
{
	char	*prefix;
	char	*suffix;
	char	**sub_str;
}	t_wildcard_parameters;

typedef struct s_wildcard_node_expansion
{
	t_token_list			*w_head;
	t_token_list			*base_next;
	t_token_list			*cur;
	DIR						*cur_dir;
	char					*w_str;
	t_wildcard_parameters	w_para;
}	t_wildcard_node_expasion;

bool			matches_wildcard(char *str, t_wildcard_parameters *w_para);
t_result		fill_wildcard_data(char *wldcd_str,
					t_wildcard_parameters *w_para);
// void			print_wildcard_data(t_wildcard_parameters *data);//? not used?
t_result		wildcards(t_token_list *name);
char			*next_file_name(DIR *dir, bool hidden);
void			clean_wildcard_data(t_wildcard_parameters *w_para);
t_result		trim_identifiers(t_token_list *name, bool set_this_true);

#endif	//EXPANSION_H
