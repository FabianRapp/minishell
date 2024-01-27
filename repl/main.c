/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 11:00:27 by frapp             #+#    #+#             */
/*   Updated: 2024/01/27 03:39:13 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parser.h"

// updates the cur_path in the path_ob, if its NULL all paths have been checked
// frees the old path
void	next_path(t_path *path_ob)
{
	if (!path_ob)
		return ;
	my_free((void **)&(path_ob->cur_path));
	path_ob->cur_path = NULL;
	if (!(path_ob->all_paths) || !(path_ob->all_paths)[path_ob->read_postion])
	{
		path_ob->cur_path = NULL;
		return ;
	}
	path_ob->position = path_ob->read_postion;
	while ((path_ob->all_paths)[path_ob->read_postion] != ':'
		&& (path_ob->all_paths)[path_ob->read_postion])
	{
		path_ob->read_postion++;
	}
	path_ob->cur_path = ft_strndup(path_ob->all_paths + path_ob->position,
		path_ob->read_postion - path_ob->position);
	while ((path_ob->all_paths)[path_ob->read_postion] == ':')
	{
		path_ob->read_postion++;
	}
}

void	init_path(t_path *path_ob)
{
	path_ob->all_paths = getenv("PATH");
	path_ob->cur_path = NULL;
	path_ob->read_postion = 0;
	path_ob->position = 0;
	next_path(path_ob);
}

// char	*find_path(char *command_name)
// {
// 	t_path	path_ob;

// 	init_path(&path_ob);
// }

// frees the given string, returns the expanded one




void	run_command(t_ast *command_data)
{
	expand_vars_command(command_data);
	//find_path();
}


int	main(void)
{
	t_ast	*ast;
	char	*input;

	input = readline("minishell: ");
	while (input != NULL)
	{
		if (*input)
		{
			add_history(input);
			ast = parser(input);
			print_ast(ast);
			free_ast(ast);
			system("leaks minishell");
			//run_command();
		}
		free(input);
		input = readline("minishell: ");
	}
	return (0);
}
