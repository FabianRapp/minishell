/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:00:52 by frapp             #+#    #+#             */
/*   Updated: 2024/02/23 16:54:54 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "../../headers/minishell.h"

// word_splitting.c
t_token_list	*word_splitting(t_token_list *list);
t_token_list	*insert_whitespace_before(t_token_list *before);

// utils1.c
bool			env_to_word_token(t_token *token);
bool			pidreq_to_literal_token(t_env *env, t_token *token);
char			*env_var_to_str(char *env_var);
bool			move_excess_name_to_arg(t_ast *ast);

// utils2.c
bool			expand_interpreted(t_token *token, t_env *env);
char			*expand_dollar(char *dollar_str, t_env *env, int *index);
t_token_list	*remove_non_literals(t_token_list *list);

// utils3.c
char			*get_last_exit_str(void);

// wildcards:

typedef struct s_wildcard_data
{
	char	*prefix;
	char	*suffix;
	char	**sub_str;
}	t_wildcard_data;

// repl_wildcards_main.c
t_result	expand_wildcards(t_ast *ast);


// repl_wildcards_utils1.c
char			*next_file_name(DIR *dir);

// repl_fill_wildcard_data.c
void		fill_wildcard_data(char *wildcard_str, t_wildcard_data *w_data);
void		clean_wildcard_data(t_wildcard_data *data);
bool		matches_wildcard(char *str, t_wildcard_data *w_data);

#endif