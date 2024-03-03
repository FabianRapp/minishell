/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:00:52 by frapp             #+#    #+#             */
/*   Updated: 2024/03/04 00:31:02 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "../../headers/minishell.h"

typedef enum e_result	t_result;

# ifndef RETURN_NOW
#  define RETURN_NOW 1
# endif

# ifndef CONTINUE
#  define CONTINUE 2
# endif

# ifndef GO_ON
#  define GO_ON 3
# endif

t_result		merge_literals(t_token_list *node);

// word_splitting.c
t_result		word_splitting(t_token_list **list);
t_result		insert_whitespace_before(t_token_list **head);

// utils1.c
t_result		env_to_word_token(t_token *token);
t_result		pidreq_to_literal_token(t_env *env, t_token *token);
t_result		add_dollar(t_token *token);
int				check_empty_arg(t_arg *last, t_arg **cur,
					t_ast *ast, t_arg **base_arg);

// utils2.c
t_result		expand_interpreted(t_token *token, t_env *env);
char			*expand_dollar(char *dollar_str, t_env *env, int *index);
t_token_list	*remove_non_literals(t_token_list *list);
t_result		expansion_move_to_arg(t_arg **next_arg, t_token_list *list);

// utils3.c
t_result		merge_literals(t_token_list *node);
t_token_list	*expand_list_here_doc(t_env *env, t_token_list *list);

// wildcards:
typedef struct s_wildcard_parameters
{
	char	*prefix;
	char	*suffix;
	char	**sub_str;
}	t_wildcard_parameters;

typedef struct s_wildcard_node_expansion
{
	t_token_list			*base_next;
	t_token_list			*cur;
	DIR						*cur_dir;
	t_wildcard_parameters	w_para;
	char					*w_str;
	t_token_list			*w_head;
}	t_wildcard_node_expasion;

// repl_wildcards_main.c
t_result		wildcards(t_token_list *name);

// repl_wildcards_utils1.c
char			*next_file_name(DIR *dir);
void			clean_wildcard_data(t_wildcard_parameters *data);

// repl_fill_wildcard_data.c
t_result		fill_wildcard_data(char *wildcard_str,
					t_wildcard_parameters *w_para);
bool			matches_wildcard(char *str, t_wildcard_parameters *w_para);

#endif