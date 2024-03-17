/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 08:00:49 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/17 01:58:54 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
behavior from bash:

bash-3.2$ echo -n -n -n hello
hellobash-3.2$ echo -n hello
hellobash-3.2$ echo -nnnnn hello
hellobash-3.2$ echo -n nnn hello
nnn hellobash-3.2$ echo -n -n  hello
hellobash-3.2$ echo -n -n  hello

bash-3.2$ echo he$llo
he
bash-3.2$
*/

//TODO: i need to add the -n

#include "../../headers/minishell.h"
#include "../../headers/eval.h"

static bool	is_the_n_option(char *arg)
{
	int	i;

	i = 1;
	if (arg[0] != '-')
		return (false);
	while (arg[i] && arg[i] == 'n')
		i++;
	if (arg[i] == '\0')
		return (true);
	else
		return (false);
}

void	ft_echo(t_ast *ast)
{
	t_arg	*cur_arg;
	char	*str_value;
	bool	no_new_line;

	no_new_line = false;
	cur_arg = ast->arg;
	if (!cur_arg)
		printf("\n");
	while (cur_arg && cur_arg->next && cur_arg->name->token->type != T_EOF)
	{
		str_value = cur_arg->name->token->str_data;
		if (is_the_n_option(str_value))
			no_new_line = true;
		else
		{
			if (str_value)
				printf("%s ", str_value);
		}
		cur_arg = cur_arg->next;
	}
	if (!cur_arg)
		str_value = NULL;
	else
		str_value = cur_arg->name->token->str_data;
	if (no_new_line == true)
	{
		if (str_value)
			printf("%s", str_value);
	}
	else if (str_value)
		printf("%s\n", str_value);
	ast->exit_status = 0;
	set_last_exit(0);
}
