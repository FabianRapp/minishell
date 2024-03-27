/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 12:26:54 by frapp             #+#    #+#             */
/*   Updated: 2024/03/27 12:30:40 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static bool	unopend_quote(char *line)
{
	bool	except;
	int		i;
	char	quote_type;

	i = 0;
	except = false;
	quote_type = 0;
	while (line && line[i])
	{
		if (quote_type == line[i] && !except)
			quote_type = 0;
		else if (!except && ! quote_type)
		{
			if (line[i] == '\'' || line[i] == '\"')
				quote_type = line[i];
		}
		if (!except && line[i] == '\\')
			except = true;
		else
			except = false;
		i++;
	}
	if (quote_type || (line && i && line[i - 1] == '\\'))
		return (true);
	return (false);
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
