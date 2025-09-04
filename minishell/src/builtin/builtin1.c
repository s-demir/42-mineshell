/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:04:56 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/24 04:50:11 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_pwd_env(char *new_path, t_env *env, int unused)
{
	char	**exp_args;
	int		tmp;

	(void)unused;
	exp_args = galloc(3 * sizeof(char *));
	if (!exp_args)
		return ;
	exp_args[0] = strcopy("export");
	exp_args[1] = build_pwd_string(new_path);
	if (!exp_args[1])
	{
		return ;
	}
	exp_args[2] = NULL;
	unset_or_export_cmd(exp_args, env, NULL, &tmp);
}

int	change_current_directory(char *path, t_env *env)
{
	int	status;
	int	a;

	if (!path || !string_length(path, '\0'))
	{
		a = locate_env_var_index(env, "HOME");
		if (a >= 0)
			status = chdir(env->parsed_env[a][1]);
		else
			status = -1;
	}
	else
		status = chdir(path);
	return (status);
}

char	*get_current_working_directory(int size, int tries, int fd)
{
	char	*buffer;

	buffer = galloc(size);
	if (!buffer)
		return (NULL);
	if (!getcwd(buffer, size))
	{
		if (tries < 10)
			return (get_current_working_directory(size + 50, tries + 1, fd));
		else
		{
			ft_putendl_fd("cd: error retrieving current directory: getcwd: "
				"cannot access parent directories: "
				"No such file or directory", fd);
			return (NULL);
		}
	}
	return (buffer);
}

char	***sort_tha_array(char ***array, int size)
{
	int		index;
	int		next_index;
	char	**temp;

	index = 0;
	while (index < size - 1)
	{
		next_index = index + 1;
		if (compare_string_order(array[index][0], array[next_index][0]))
		{
			temp = array[index];
			array[index] = array[next_index];
			array[next_index] = temp;
		}
		index++;
	}
	if (!is_array_sorted(array, size))
		return (sort_tha_array(array, size));
	return (array);
}

void	print_export_declaration_to_fd(t_env *env, int *_out_fd)
{
	char	***new_array;
	int		a;

	a = 0;
	while (env->parsed_env[a] != 0)
		a++;
	if (!a)
		return ;
	new_array = duplicate_env_structure(env, a, 2, -1);
	new_array[a] = 0;
	new_array = sort_tha_array(new_array, a);
	print_export_vars(new_array, a, _out_fd[1]);
}
