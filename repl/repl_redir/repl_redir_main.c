/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_redir_main.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 04:20:36 by frapp             #+#    #+#             */
/*   Updated: 2024/03/11 17:11:35 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "repl_redir_internals.h"

t_result	handle_redir_out(t_ast *ast, t_fd_pair **fds, t_redir *redir, bool append)
{
	t_fd_pair	new_fd_pair;
	int			base_fd;

	base_fd = redir->left_redir_arg;
	new_fd_pair = redir_fd_write(redir->arg->name->token->str_data, append, base_fd);
	if (errno)
	{
		print_error(true, NULL, NULL, strerror(errno));
		ast->exit_status = 1;
		set_last_exit(1);
		errno = 0;
		return (ERROR);
	}
	if (handle_open_error(ast, &new_fd_pair) == ERROR)
	{
		ast->exit_status = get_last_exit();
		return (ERROR);
	}
	(*fds) = add_fd_pair((*fds), new_fd_pair);
	if (!(*fds))
	{
		ast->exit_status = get_last_exit();
		return (io_data(SET_NEW_FDS, NULL), ERROR);
	}
	return (SUCCESS);
}

t_result	handle_redir_in(t_ast *ast, t_fd_pair **fds, t_redir *redir, bool in_out)
{
	t_fd_pair	new_fd_pair;
	int			base_fd;

	base_fd = redir->left_redir_arg;
	new_fd_pair = redir_read(redir->arg->name->token->str_data, base_fd, in_out);
	if (errno)
	{
		print_error(true, NULL, NULL, strerror(errno));
		ast->exit_status = 1;
		set_last_exit(1);
		errno = 0;
		return (ERROR);
	}
	if (handle_open_error(ast, &new_fd_pair) == ERROR)
	{
		ast->exit_status = get_last_exit();
		return (ERROR);
	}
	(*fds) = add_fd_pair((*fds), new_fd_pair);
	if (!(*fds))
	{
		ast->exit_status = get_last_exit();
		return (io_data(SET_NEW_FDS, NULL), ERROR);
	}
	return (SUCCESS);
}

// Handles the redirection for here documents during command execution.
// Creates a new file descriptor pair for the base and the here-doc's pipe read end.
// Adds the file descriptor pair to the array of fds for redirection management.
t_result	repl_handle_here_doc(t_ast *ast, t_redir *redir, t_fd_pair **fds)
{
	t_fd_pair	new_fd_pair;

	new_fd_pair.base_fd = redir->left_redir_arg;
	if (new_fd_pair.base_fd == INIT_VAL)
		new_fd_pair.base_fd = 0;
	new_fd_pair.overload_with_fd = ft_atoi(redir->token_str_data + 4);
	ft_free((void **)&(redir->token_str_data));
	//printf("overload with: %d\n", new_fd_pair.overload_with_fd);
	(*fds) = add_fd_pair((*fds), new_fd_pair);
	if (!(*fds))
	{
		ast->exit_status = get_last_exit();
		return (ft_free((void **)&(redir->token_str_data)), io_data(SET_NEW_FDS, NULL), ERROR);
	}
	return (ft_free((void **)&(redir->token_str_data)), SUCCESS);
}

t_result	resolve_redirs(t_ast *ast)
{
	t_redir		*redir;
	int			base_fd;
	//t_fd_pair	new_fd_pair;
	t_fd_pair	*fds;

	redir = ast->redir;
	fds = NULL;
	// new_fd_pair.overload_with_fd = ast->pipe[WRITE];
	// new_fd_pair.base_fd = WRITE;
	// fds = add_fd_pair(fds, new_fd_pair);
	// if (!fds)
	// 	return (ERROR);
	// new_fd_pair.overload_with_fd = ast->pipe[READ];
	// new_fd_pair.base_fd = READ;
	// fds = add_fd_pair(fds, new_fd_pair);
	// if (!fds)
	// 	return (ERROR);
	while (redir)
	{
		if (!check_valid_arg(ast, redir))
			return (io_data(SET_NEW_FDS, NULL), ERROR);
		base_fd = redir->left_redir_arg;
		if (redir->type == REDIR_OUT)
		{
			if (handle_redir_out(ast, &fds, redir, false) == ERROR)
				return (ERROR);
		}
		else if (redir->type == REDIR_APPEND)
		{
			if (handle_redir_out(ast, &fds, redir, true) == ERROR)
				return (ERROR);
		}
		else if (redir->type == REDIR_IN || redir->type == REDIR_IN_OUT)
		{
			if (handle_redir_in(ast, &fds, redir, (redir->type == REDIR_IN_OUT)) == ERROR)
				return (ERROR);
		}
		// TODO
		else if (redir->type == HERE_DOC)
		{
			if (repl_handle_here_doc(ast, redir, &fds) == ERROR)
				return (ERROR);
		}
		redir = redir->next;
	}
	io_data(SET_NEW_FDS, fds);
	return (SUCCESS);
}
