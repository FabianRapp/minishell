/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 12:26:54 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 18:30:12 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static t_result	handle_brackets(char *line, int *bracket_lvl, int i)
{
	if (line[i] == '(')
		(*bracket_lvl)++;
	if (line[i] == ')')
		(*bracket_lvl)--;
	if (*bracket_lvl < 0)
	{
		ft_fprintf(2, "%s : syntax error near unexpected token `)'\n",
			SHELL_NAME);
		line[0] = 0;
		return (set_last_exit(2), ERROR);
	}
	return (SUCCESS);
}

static bool	unopend_quote(char *line)
{
	bool	except;
	int		i;
	char	quote_type;
	int		bracket_lvl;

	except = false;
	quote_type = 0;
	bracket_lvl = 0;
	i = -1;
	while (line && line[++i])
	{
		if (handle_brackets(line, &bracket_lvl, i) == ERROR)
			return (false);
		if (quote_type == line[i] && !except)
			quote_type = 0;
		else if (!except && ! quote_type)
		{
			if (line[i] == '\'' || line[i] == '\"')
				quote_type = line[i];
		}
		except = (!except && line[i] == '\\');
	}
	if (line && i && (line[i - 1] == '|' || line[i - 1] == '&'))
		return (true);
	return (quote_type || except || (bracket_lvl && !TESTER));
}

static char	*handle_multi_line(char *line)
{
	char	*tmp;
	char	*tmp2;

	while (unopend_quote(line))
	{
		ft_strjoin_inplace(&line, "\n");
		if (!isatty(0))
		{
			tmp = get_next_line(0, false);
			tmp2 = ft_strtrim(tmp, "\n");
			ft_strjoin_inplace(&line, tmp2);
			free(tmp2);
		}
		else
		{
			tmp = readline("> ");
			if (!tmp)
				break ;
			ft_strjoin_inplace(&line, tmp);
		}
		ft_free((void **)&tmp);
		line_counter();
	}
	return (line);
}

char	*ft_read_line(char *header)
{
	char	*line;
	char	*tmp;

	redisplay_prompt(true, true);
	tmp = NULL;
	if (!isatty(0))
	{
		tmp = get_next_line(0, false);
		line = ft_strtrim(tmp, "\n");
		ft_free((void **)&tmp);
	}
	else
		line = readline(header);
	line = handle_multi_line(line);
	redisplay_prompt(true, false);
	return (line);
}
