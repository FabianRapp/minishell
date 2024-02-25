/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:00:52 by frapp             #+#    #+#             */
/*   Updated: 2024/02/25 01:25:06 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "../../headers/minishell.h"

typedef enum e_result	t_result;

// word_splitting.c
t_result		word_splitting(t_token_list **list);
t_result		insert_whitespace_before(t_token_list **head);

// utils1.c
t_result		env_to_word_token(t_token *token);
t_result		pidreq_to_literal_token(t_env *env, t_token *token);

// utils2.c
t_result		expand_interpreted(t_token *token, t_env *env);
char			*expand_dollar(char *dollar_str, t_env *env, int *index);
t_token_list	*remove_non_literals(t_token_list *list);

// utils3.c
char			*get_last_exit_str(void);

// wildcards:

typedef struct s_wildcard_parameters
{
	char	*prefix;
	char	*suffix;
	char	**sub_str;
}	t_wildcard_parameters;

// repl_wildcards_main.c
t_result		wildcards_expand_name(t_token_list *name);

// repl_wildcards_utils1.c
char			*next_file_name(DIR *dir);
void		clean_wildcard_data(t_wildcard_parameters *data);

// repl_fill_wildcard_data.c
void		fill_wildcard_data(char *wildcard_str, t_wildcard_parameters *w_para);
bool		matches_wildcard(char *str, t_wildcard_parameters *w_para);

#endif