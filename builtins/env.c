/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mevangel <mevangel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 17:36:06 by mevangel          #+#    #+#             */
/*   Updated: 2024/03/27 02:29:41 by mevangel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	handle_args(t_arg *arg, t_shared_data *shared_data)
{
	t_ast			ast;
	const t_ast		init_val = {COMMAND, NULL, NULL, NULL, NULL, NULL,
		INIT_VAL, NULL, INIT_VAL, INIT_VAL, INIT_VAL, INIT_VAL, false};
	t_shared_data	new_shared_data;
	char			**empty_env;

	empty_env = ft_calloc(40, sizeof(char *));
	new_shared_data = *shared_data;
	new_shared_data.envs = &empty_env;
	ast = init_val;
	ast.shared_data = shared_data;
	if (ft_strcmp(arg->name->token->str_data, "-i") == 0)
	{
		ast.shared_data = &new_shared_data;
		arg = arg->next;
	}
	ast.name = arg->name;
	ast.name->next = NULL;
	ast.arg = arg->next;
	run_node(&ast);
	if (ast.exit_status != INIT_VAL)
		return (free(empty_env), ast.exit_status);
	waitpid(ast.pid, &(ast.exit_status), 0);
	set_last_exit(WEXITSTATUS(ast.exit_status));
	return (free(empty_env), WEXITSTATUS(ast.exit_status));
}

int	ft_env(t_ast *ast)
{
	int		i;
	char	**tmp;
	char	*path;

	path = get_env_value(*(ast->shared_data->envs), "PATH", 0, 0);
	if (!path)
	{
		print_error(true, "env", NULL, "No such file or directory");
		ft_cur_exit(ast, 127);
		return (free(path), 0);
	}
	if (ast->arg)
	{
		ast->exit_status = handle_args(ast->arg, ast->shared_data);
		return (free(path), 0);
	}
	if (ast && ast->shared_data->envs)
		tmp = *(ast->shared_data->envs);
	else
		tmp = *(get_env_list(NULL));
	i = -1;
	while (tmp[++i])
		printf("%s\n", tmp[i]);
	ft_cur_exit(ast, 0);
	return (free(path), 0);
}
