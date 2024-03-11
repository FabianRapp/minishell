/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:29:13 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/11 01:49:10 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
export VARIABLE=value 
command1		->  with export (comparing to just env) the variable becomes 
					available not only for the "command1" but also for any 
					subsequent commands in the current shell session 

the "export" alone, prints a list of all the environment variables along with 
their values in the formatting: 
		declare -x HOME="/Users/mevangel"
		declare -x LANG="en_US.UTF-8"
		declare -x LOGNAME="mevangel"
		declare -x MallocNanoZone="0"
		etc... 
*/

#include "../../headers/minishell.h"
#include "../../headers/eval.h"

static void	sort_env_array(char **env, int size)
{
	int		i;
	char	*tmp;
	bool	sorting_done;

	tmp = 0;
	sorting_done = false;
	while (sorting_done == false)
	{
		sorting_done = true;
		i = -1;
		while (++i < size - 1)
		{
			if (ft_strcmp(env[i], env[i + 1]) > 0)
			{
				tmp = env[i];
				env[i] = env[i + 1];
				env[i + 1] = tmp;
				sorting_done = false;
			}
		}
	}
}

//TODO i think i must print them as: declare -x USER="mevangel" -with the quotes i mean. not sure.
static void	ft_export_no_args(t_ast *ast)
{
	int		i;
	char	**sorted_env;

	sorted_env = ast->envs;
	i = 0;
	while (sorted_env[i])
		i++;
	sort_env_array(sorted_env, i);
	i = -1;
	while (sorted_env[++i])
	{
		printf("declare -x ");
		printf("%s\n", sorted_env[i]);
	}
}

void	ft_export(t_ast *ast)
{
	char	**args;

	// printf("the cmd_name i got here is: %s$\n", ast->name->token->input_str);
	args = ft_split(ast->name->token->input_str, ' ');
	if (args[1] == NULL) //which means that export has no arguments
	{
		ft_export_no_args(ast);
		return ;
	}
	
	ft_free_2darr(args);
}