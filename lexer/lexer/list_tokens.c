/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 09:26:13 by frapp             #+#    #+#             */
/*   Updated: 2024/01/18 11:53:36 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/lexer.h"
#include "internals.h"

// this will leak if (*last) leads to an existing list but (*first) == NULL
void	circ_lst_add_back(t_token **last, t_token **first, t_token *cur)
{
	if (!last)
		return ;
	if (!*first)
	{
		*last = cur;
		*first = cur;
		cur->last = NULL;
		cur->next = NULL;
		return ;
	}
	(*last)->next = cur;
	cur->last = *last;
	(*first)->last = cur;
	cur->next = *first;
}

// each node has to be freed and each tokens values as a normal token would
t_token	*token_list(char *str)
{
	t_token			*cur;
	t_token			*first;
	t_token			*last;
	t_lexer			lexer;

	first = NULL;
	lexer = new_lexer(str);
	cur = NULL;
	while (!cur || cur->type != T_EOF)
	{
		last = cur;
		cur = ft_calloc(1, sizeof(t_token));
		if (!cur)
			return (cleanup(), NULL);
		*cur = next_new_token(&lexer);
		circ_lst_add_back(&last, &first, cur);
	}
	return (first);
}

// returns false if the T_EOF token is reached
bool	list_right(t_token **token)
{
	*token = (*token)->next;
	if ((*token)->type == T_EOF)
		return (false);
	return (true);
}

// returns false if the T_EOF token is reached
bool	list_left(t_token **token)
{
	*token = (*token)->last;
	if ((*token)->type == T_EOF)
		return (false);
	return (true);
}

void	free_token(t_token *token)
{
	if (!token)
		return ;
	if (token->str)
		free(token->str);
	free(token);
}
/*
	- returns 1 on success, unless:
	- returns 0 if the token list is empty afterwards
	- returns -1 if the list is corrupted and does not free in that case
		(does not activly check for list intagret)
*/
int	remove_token(t_token *token)
{
	if (!token)
		return (0);
	// after this only one or none tokens left in the list
	if (token->last == token->next)
	{
		if (!(token->last)) // list empty afterwards
			return (free_token(token), 0);
		token->last->last = NULL;
		token->last->next = NULL;
		return (free_token(token), true);
	}
	// duo to last condtion this means only one is null
	if (!token->last || !token->next)
		return (-1);
	// now only cases where there are diffrent last and next elements left
	token->last->next = token->next;
	token->next->last = token->last;
	free_token(token);
	return (1);
}
