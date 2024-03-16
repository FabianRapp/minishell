/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_redir_internals.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 04:21:19 by frapp             #+#    #+#             */
/*   Updated: 2024/03/11 17:11:45 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPL_REDIR_INTERNALS_H
# define REPL_REDIR_INTERNALS_H

#  include "../../headers/minishell.h"

// main interface
t_result	resolve_redirs(t_ast *ast);
int			extend_fd_array(t_fd_pair **fds);
t_fd_pair	redir_read(char *file, int base_fd, bool in_out);
t_fd_pair	redir_fd_write(char *file, bool append, int base_fd);

// utils
t_fd_pair	*add_fd_pair(t_fd_pair *fds, t_fd_pair new_fd_pair);

// error handlers
t_result	handle_open_error(t_ast *ast, t_fd_pair *fd_pair);
t_result	handle_closed_fd_arg(t_fd_pair *fds, t_fd_pair *new_fd_pair);
void		*add_fd_pair_error(t_fd_pair *fds, t_fd_pair new_fd_pair);
t_result	check_valid_arg(t_ast *ast, t_redir *redir);

#endif