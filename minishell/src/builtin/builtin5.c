/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 12:20:06 by amsagman          #+#    #+#             */
/*   Updated: 2025/08/24 12:20:09 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_env_command(t_env *env, int con, int *_out_fd)
{
	int	a;

	a = -1;
	if (con)
		print_export_declaration_to_fd(env, _out_fd);
	else
	{
		while (env->parsed_env[++a])
			print_env_var_to_fd(env->parsed_env[a][0],
				env->parsed_env[a][1], _out_fd[1]);
	}
	return (0);
}

static int	handle_pwd_command(t_env *env, int *_out_fd)
{
	char	*logical_pwd;
	char	*abs_pwd;

	logical_pwd = get_logical_pwd(env);
	if (logical_pwd)
	{
		ft_putendl_fd(logical_pwd, _out_fd[1]);
		return (0);
	}
	abs_pwd = get_current_working_directory(100, 5, _out_fd[1]);
	if (abs_pwd)
	{
		ft_putendl_fd(abs_pwd, _out_fd[1]);
		update_logical_pwd(env, abs_pwd);
		return (0);
	}
	return (256);
}

int	env_or_pwd_cmd(char *_cmd, t_env *env, int con, int *_out_fd)
{
	if (str_cmp(_cmd, "env", NULL))
		return (handle_env_command(env, con, _out_fd));
	return (handle_pwd_command(env, _out_fd));
}

char	**export_cmd(char **_cmd, t_env *env, int *_out_fd, int **s)
{
	int	a;
	int	result;

	a = 1;
	while (_cmd[a])
	{
		result = export_statment_check(_cmd[a]);
		if (result > 0)
			process_export_assignment(_cmd[a], result, env);
		else if (result == 0)
		{
			update_environment_var(_cmd[a], env);
		}
		else
		{
			ft_putstr_fd("export: `", _out_fd[1]);
			ft_putstr_fd(_cmd[a], _out_fd[1]);
			ft_putendl_fd("': not a valid identifier", _out_fd[1]);
			**s = 1;
		}
		a++;
	}
	return (_cmd);
}

void	process_export_assignment(char *cmd, int result, t_env *env)
{
	if (cmd[result] == '+' && cmd[result + 1] == '=')
		handle_append_export(cmd, env, result);
	else
		update_environment_var(cmd, env);
}
