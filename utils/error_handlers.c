/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 00:23:28 by frapp             #+#    #+#             */
/*   Updated: 2024/03/25 00:41:46 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

t_result	errno_to_result(void)
{
	if (errno)
		return (ERROR);
	return (SUCCESS);
}

t_result	set_errno_as_exit(t_ast *ast, bool msg)
{
	ast->exit_status = errno;
	set_last_exit(errno);
	if (errno)
	{
		if (msg)
			print_error(true, NULL, NULL, strerror(errno));
		errno = 0;
		return (ERROR);
	}
	return (SUCCESS);
}

void	print_error(bool shell_name, char *command_name, char *arg, char *str)
{
	if (shell_name)
		ft_fprintf(2, "%s: ", SHELL_NAME);
	if (command_name)
		ft_fprintf(2, "%s: ", command_name);
	if (arg)
		ft_fprintf(2, "%s: ", arg);
	if (str)
		ft_fprintf(2, "%s", str);
	ft_fprintf(2, "\n");
}

void	print_error_weird_quotes(bool shell_name,
	char *command_name, char *arg, char *str)
{
	if (shell_name)
		ft_fprintf(2, "%s: ", SHELL_NAME);
	if (command_name)
		ft_fprintf(2, "%s: ", command_name);
	if (arg)
		ft_fprintf(2, "`%s': ", arg);
	if (str)
		ft_fprintf(2, "%s", str);
	ft_fprintf(2, "\n");
}

// when this function is given true the current process
// will exit after the next cleanup
bool	full_exit_status(bool set_full_exit)
{
	static bool	full_exit = false;

	if (set_full_exit)
		full_exit = true;
	return (full_exit);
}
