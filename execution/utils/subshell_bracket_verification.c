/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_bracket_verification.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:38:49 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/26 16:44:37 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static int	skip_quotes_subshell(char *input, int i)
{
	int		i2;
	bool	except;

	except = false;
	i2 = 1;
	while (input[i + i2] && !(!except && input[i + i2] != input[i]))
	{
		if (input[i + i2] == '\\')
		{
			i2++;
			except = true;
		}
		i2++;
	}
	i += i2;
	return (i);
}

static t_result	count_brackets_subshell(char *input)
{
	int		bracket_lvl;
	int		i;
	bool	except;

	bracket_lvl = 1;
	i = 0;
	except = false;
	while (input[i])
	{
		if (!bracket_lvl)
			return (SUCCESS);
		if (input[i] == '"' && !except)
			i = skip_quotes_subshell(input, i);
		if (input[i] == '(' && !except)
			bracket_lvl++;
		if (input[i] == ')' && !except)
			bracket_lvl--;
		except = (input[i] == '\\' && !except);
		i++;
	}
	return (ERROR);
}

t_result	check_brackets(t_ast *ast, char *input)
{
	char	*tmp;

	if (!input)
		return (print_error(true, NULL, NULL, "syntax error near"
				" unexpected token `)'"), ft_cur_exit(ast, 2), ERROR);
	if (*(input++) != '(')
		return (SUCCESS);
	if (count_brackets_subshell(input) == SUCCESS)
		return (SUCCESS);
	tmp = ft_strndup(input, ft_strlen(input) - 1);
	ft_fprintf(2, "%s: ((: %s: syntax error in expression (error token is"
		" \"%s\")\n", SHELL_NAME, tmp, input);
	return (ft_cur_exit(ast, 1), free(tmp), ERROR);
}
