/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_redir.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 05:35:46 by frapp             #+#    #+#             */
/*   Updated: 2024/03/16 22:18:09 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/parser.h"
#include "../internals_parser.h"
#include "../../headers/lexer.h"

char	*parser_expand_dollar(char *dollar_str, int *index)
{
	char	*env_var;
	char	*return_str;

	if (!*(dollar_str + 1))
		return ((*index)++, ft_strdup("$"));
	else if (*(dollar_str + 1) == '?')
		return_str = get_last_exit_str();
	else if (*(dollar_str + 1) == '$')
		return_str = ft_itoa(ft_pid(0));
	else if (ft_isdigit(*(dollar_str + 1)))
		return ((*index)++, ft_strdup("$"));
	else if (name_len(dollar_str + 1) == 0)
		return ((*index)++, ft_strdup("$"));
	else if (ft_isalpha(*(dollar_str + 1)) || *(dollar_str + 1) == '_')
	{
		*index += name_len(dollar_str + 1) + 1;
		env_var = ft_strndup(dollar_str + 1, name_len(dollar_str + 1));
		if (!env_var)
			return (NULL);
		//return_str = ft_strdup(getenv(env_var));
		return_str = get_env_value(NULL, env_var);
		if (!return_str)
			return_str = ft_calloc(1, 1);
		return (free(env_var), return_str);
	}
	else
		return ((*index)++, ft_strdup(""));
	*index += 2;
	return (return_str);
}

char	*parser_expand_line(char *line)
{
	char	*new_line;
	int		i;
	char	*dollar;
	char	*temp;

	new_line = ft_calloc(1, 1);
	if (!new_line)
		return (free(line), NULL);
	dollar = NULL;
	i = 0;
	while (line[i] && line[i] != '\n')
	{
		if (line[i] != '$')
		{
			ft_strjoin_inplace_char(&new_line, line[i]);
		}
		else
		{
			temp = parser_expand_dollar(line + i, &i);
			if (!temp)
				return (free(new_line), free(line), NULL);
			ft_strjoin_inplace(&new_line, temp);
			free(temp);
		}
		i++;
	}
	free(line);
	if (ft_strjoin_inplace_char(&new_line, '\n') == ERROR)
	{
	}
	return (new_line);
}

//for parser_resovle_here_doc
t_result	parser_resolve_here_doc(char *termination, int pipe_fd[2], bool expand_vars)
{
	char	*line;
	bool	start;
	int		count;

	line = NULL;
	start = true;
	while (line || start)
	{
		start = false;
		free(line);
		if (!isatty(fileno(stdin)))
			line = get_next_line(fileno(stdin));
		else
		{
			//line = ft_read_line(">");
			line = readline(">");
			ft_strjoin_inplace(&line, "\n");
		}
		count = line_counter();
		if (!line && errno)
		{
			errno = 0;
			return (ERROR);
		}
		if (!line || !*line)
		{
			ft_fprintf(2, "%s: warning: here-document at line %d delimited by end-of-file (wanted `%s')", SHELL_NAME, count, termination);
			return (SUCCESS);
		}
		if (ft_strcmp(line, termination) == 0)
		{
			ft_free((void **)&line);
			return (SUCCESS);
		}
		if (expand_vars)
			line = parser_expand_line(line);
		if (ft_fprintf(pipe_fd[WRITE], "%s", line) == -1)
			return (set_last_exit(errno), ERROR);
	}
	free(line);
	if (errno)
		set_last_exit(errno);
	else
		set_last_exit(1);
	return (ERROR);
}

// Initializes a pipe and captures input until
//	a termination string is encountered.
// Stores the read-end file descriptor as a
//	string in the redir structure for later use.
// Writes each line of input to the write-end
//	of the pipe, excluding the termination line.
t_result	parser_resovle_here_doc(t_redir *redir)
{
	int				pipe_fd[2];
	char			*termination;
	char			*temp;

	termination = ft_strjoin(redir->token_str_data, "\n");
	if (!termination)
		return (ERROR);
	if (pipe(pipe_fd) == -1)
		return (set_last_exit(errno), free(termination), ERROR);
	free(redir->token_str_data);
	redir->token_str_data = ft_itoa(pipe_fd[READ]);
	if (!redir->token_str_data)
		return (close(pipe_fd[READ]), close(pipe_fd[WRITE]),
			free(termination), ERROR);
	temp = NULL;
	if (redir->token_str_data)
		temp = ft_strjoin("<<<<", redir->token_str_data);
	free(redir->token_str_data);
	redir->token_str_data = temp;
	if (!redir->token_str_data
		|| parser_resolve_here_doc(termination, pipe_fd, !(redir->here_doc_literal)) == ERROR)
		return (close(pipe_fd[READ]), close(pipe_fd[WRITE]),
			free(termination), ERROR);
	return (close(pipe_fd[WRITE]), free(termination), SUCCESS);
}

t_result	append_redir(t_ast *ast_node, t_parser *args, t_redir **cur_redir)
{
	if (*cur_redir)
	{
		(*cur_redir)->next = ft_calloc(1, sizeof(t_redir));
		if (!(*cur_redir)->next)
			return (ERROR);
		*cur_redir = (*cur_redir)->next;
	}
	else
	{
		*cur_redir = ft_calloc(1, sizeof(t_redir));
		if (!*cur_redir)
			return (ERROR);
		ast_node->redir = *cur_redir;
	}
	(*cur_redir)->type = args->token->type;
	(*cur_redir)->token_str_data = ft_strdup(args->token->str_data);
	if (args->token->str_data && !(*cur_redir)->token_str_data)
		return (ERROR);
	
	(*cur_redir)->arg = append_arg(args->arg, (*cur_redir)->arg);
	if (!((*cur_redir)->arg) && (*cur_redir)->type != HERE_DOC)
		return (ERROR);
	(*cur_redir)->here_doc_literal = args->token->here_doc_arg_literal;
	(*cur_redir)->left_redir_arg = INIT_VAL;
	if (args->token->left_redir_arg != NULL)
		(*cur_redir)->left_redir_arg = ft_atoi(args->token->left_redir_arg);
	return (SUCCESS);
}
