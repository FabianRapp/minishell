/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:34:51 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/25 01:37:20 by frapp            ###   ########.fr       */
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

void	print_error_redir_arg(t_lexer *lexer)
{
	int		base_std_err;
	int		temp_pipe[2];
	t_token	*error_token;

	set_last_exit(2);
	base_std_err = dup(2);
	pipe(temp_pipe);
	close(temp_pipe[READ]);
	dup2(temp_pipe[WRITE], 2);
	close(temp_pipe[WRITE]);
	error_token = next_new_token(lexer, true);
	dup2(base_std_err, 2);
	close(base_std_err);
	if (error_token)
		print_error(true, NULL, NULL, type_to_error(error_token->type));
	free_token(error_token);
}
