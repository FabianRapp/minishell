/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:29:13 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/13 22:06:03 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
export VARIABLE=value 
command1		->  with export (comparing to just env) the variable becomes 
					available not only for the "command1" but also for any 
					subsequent commands in the current shell session 

the VARIABLE name can NOT start with a number:
EXAMPLE: 	bash-3.2$ export 4abc=hello
			bash: export: `4abc=hello': not a valid identifier
but it can be a number somewhere else in the name

the "export" alone, prints a list of all the environment variables along with 
their values in the formatting: 
		declare -x HOME="/Users/mevangel"
		declare -x LANG="en_US.UTF-8"
		declare -x LOGNAME="mevangel"
		declare -x MallocNanoZone="0"
		etc... 
*/

//? if there is a lot of time, maybe check if i could handle the += for export

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
	char	*var_name;

	sorted_env = *(ast->envs);
	i = 0;
	while (sorted_env[i])
		i++;
	sort_env_array(sorted_env, i);
	i = -1;
	while (sorted_env[++i])
	{
		printf("declare -x ");
		var_name = get_env_var_name(sorted_env[i]);
		printf("%s=\"%s\"\n", var_name, get_env_value(ast, var_name));
		free(var_name);
		// printf("%s\n", sorted_env[i]);
	}
}

/*
*	In Bash, the rules for valid environment variable names are as follows:
*	Variable names must begin with a letter (a-z or A-Z) or an underscore (_).
*	Subsequent characters can be letters, numbers, or underscores.
*	Variable names are case-sensitive.
*/
bool	arg_is_valid(char *arg)
{
	int		i;
	char	*save;

	i = 0;
	save = arg;
	if (!arg)
		return (false);
	// i check first character firstly:	
	if (!(ft_isalpha((int) *arg) || *arg == '_'))
		return (print_error_addsq(true, "export", save, "not a valid identifier"), false);
	// then i continue with the rest chars of var_name, until the equal
	while (++arg && *arg && *arg != '=')
	{
		if (!(ft_isalnum((int) *arg) || *arg == '_'))
			return (print_error_addsq(true, "export", save, "not a valid identifier"), false);
	}
	//if there is not equal after the name, nothing is printed and nothing is added in env array
	if (*arg != '=')
		return (false);
	return (true);
}

void	ft_export(t_ast *ast)
{
	char	**args;
	t_arg	*cur_arg;
	char	*str_value;
	int		num;

	//printf("the cmd_name i got here is: %s$\n", ast->name->token->input_str);
	// printf("the cmd_name i got here is: %s$\n", ast->name->token->str_data);
	// ast->arg->name->token->str_data;
	num = 0;
	cur_arg = ast->arg;
	while (cur_arg && cur_arg->name->token->type != T_EOF)
	{
		//i never enter here if i write "export" with no arguments!
		str_value = cur_arg->name->token->str_data;
		// printf("current str_value is: %s\n", str_value);
		if (arg_is_valid(str_value))
			*(ast->envs) = add_env_var(ast, str_value);
		num++;
		cur_arg = cur_arg->next;
	}
	if (num == 0) //which means that export has no arguments
		ft_export_no_args(ast);
	ast->exit_status = 0;
	set_last_exit(0);
}

// ft_memmove() 

// getenv is used in the expansion

// // print_error(true, NULL, NULL, strerror(errno));
// // print_error(true, "string1", "string2", strerror(errno));
// // ft_fprintf(2, "%s %s\n", SHELL_NAME, );