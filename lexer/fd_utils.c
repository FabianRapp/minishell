/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:34:51 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/23 15:35:21 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/lexer.h"
#include "../headers/minishell.h"

char	*get_potential_fd(t_lexer *lexer)
{
	char	*left_redir_arg;
	t_lexer	lexer_backup;

	lexer_backup = *lexer;
	left_redir_arg = NULL;
	while (ft_isdigit(lexer->cur_char))
	{
		if (!ft_strjoin_inplace_char(&left_redir_arg, lexer->cur_char))
			return (NULL);
		read_char(lexer);
	}
	return (left_redir_arg);
}

char	*check_limis_potential_fd(char *left_redir_arg,
	t_lexer *lexer, t_lexer lexer_backup)
{
	if (lexer->cur_char != '<' && lexer->cur_char != '>')
		ft_free((void **)&left_redir_arg);
	else if (ft_strlen(left_redir_arg) > ft_strlen("2147483647"))
		ft_free((void **)&left_redir_arg);
	else if (ft_strlen(left_redir_arg) == ft_strlen("2147483647"))
	{
		if (ft_strcmp(left_redir_arg, "2147483647") > 0)
			ft_free((void **)&left_redir_arg);
	}
	if (!left_redir_arg)
		*lexer = lexer_backup;
	return (left_redir_arg);
}
