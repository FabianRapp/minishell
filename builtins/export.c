/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:29:13 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/20 12:10:53 by frapp            ###   ########.fr       */
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

#include "../headers/minishell.h"
#include "../headers/eval.h"

static void	sort_shared_data_array(char **env, int size)
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

static void	ft_export_no_args(t_ast *ast)
{
	int		i;
	char	**sorted_env;
	char	*var_name;
	char	*var_value;

	sorted_env = *(ast->shared_data->env_exp);
	i = 0;
	while (sorted_env[i])
		i++;
	sort_shared_data_array(sorted_env, i);
	i = -1;
	while (sorted_env[++i])
	{
		printf("declare -x ");
		if (ft_strchr(sorted_env[i], '='))
		{
			var_name = get_env_var_name(sorted_env[i]);
			var_value = get_env_value(*(ast->shared_data->envs), var_name);
			printf("%s=\"%s\"\n", var_name, var_value);
			free(var_name);
			free(var_value);
		}
		else
			printf("%s\n", sorted_env[i]);
	}
}

/*
*	In Bash, the rules for valid environment variable names are as follows:
*	Variable names must begin with a letter (a-z or A-Z) or an underscore (_).
*	Subsequent characters can be letters, numbers, or underscores.
*	Variable names are case-sensitive.
*/
int	arg_is_valid(char *arg, t_ast *ast, char *cmd_name)
{
	int		i;
	char	*save;

	i = 0;
	save = arg;
	if (!arg)
		return (0);
	// if (*arg == '\0')
	// 	return (ft_cur_exit(ast, 1), print_error_addsq(true, cmd_name,
	// 		save, "not a valid identifier"), 0);//these three last lines i don't think they made any difference
	// i check first character firstly:
	if (*arg == '-')
	{
		print_error(true, "unset", save, "invalid option");
		ft_fprintf(2, "no options supported\n");
		ast->exit_status = 2;
		set_last_exit(2);
		return (3);
	}
	if (!(ft_isalpha((int) *arg) || *arg == '_'))
		return (ft_cur_exit(ast, 1), print_error_addsq(true, cmd_name,
			save, "not a valid identifier"), 0);
	// then i continue with the rest chars of var_name, until the equal
	while (++arg && *arg && *arg != '=')
	{
		if (!(ft_isalnum((int) *arg) || *arg == '_'))
			return (ft_cur_exit(ast, 1), print_error_addsq(true, cmd_name,
				save, "not a valid identifier"), 0);
	}
	//if there is not equal after the name, nothing is printed and nothing is added in env array
	if (*arg != '=')
		return (2);
	return (1);
}

void	ft_export(t_ast *ast)
{
	t_arg	*cur_arg;
	char	*str_value;
	int		num;
	int		res;

	num = 0;
	res = 0;
	cur_arg = ast->arg;
	ft_cur_exit(ast, 0);
	while (cur_arg && cur_arg->name->token->type != T_EOF)
	{
		str_value = cur_arg->name->token->str_data;
		res = arg_is_valid(str_value, ast, "export");
		if (res == 3)
			return ;
		if (res > 0)
			*(ast->shared_data->env_exp) = new_env_list_after_add(str_value,
				*(ast->shared_data->env_exp));
		if (res == 1)
			*(ast->shared_data->envs) = new_env_list_after_add(str_value,
				*(ast->shared_data->envs));
		num++;
		cur_arg = cur_arg->next;
	}
	if (num == 0) //which means that export has no arguments
		ft_export_no_args(ast);
}

// print_error(true, NULL, NULL, strerror(errno));
// print_error(true, "string1", "string2", strerror(errno));
// ft_fprintf(2, "%s %s\n", SHELL_NAME, );