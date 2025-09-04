/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin6.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 12:20:14 by amsagman          #+#    #+#             */
/*   Updated: 2025/08/24 12:20:16 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_cd_failure(char **_cmd, t_env *env, int *_out_fd)
{
	int		a;
	char	*logical_path;
	char	*current_pwd;

	current_pwd = get_logical_pwd(env);
	if (current_pwd && _cmd[1] && str_cmp(_cmd[1], "..", NULL))
	{
		logical_path = resolve_path(current_pwd, _cmd[1]);
		if (logical_path)
		{
			update_logical_pwd(env, logical_path);
			a = locate_env_var_index(env, "PWD");
			if (a >= 0)
				remove_env_entry(env, a);
			update_pwd_env(logical_path, env, a);
		}
		ft_putendl_fd("cd: error retrieving current directory: getcwd: "
			"cannot access parent directories: "
			"No such file or directory", _out_fd[2]);
		return (0);
	}
	else
		return (report_command_error(_cmd[1], 1));
}

int	cd_cmd(char **_cmd, t_env *env, int *_out_fd)
{
	int		a;
	char	*new_path;

	if (_cmd[1] && _cmd[2])
		ft_putendl_fd("  err: cd(): Not a cd thing", _out_fd[1]);
	else
	{
		if (change_current_directory(_cmd[1], env) < 0)
			return (handle_cd_failure(_cmd, env, _out_fd));
		else
		{
			a = locate_env_var_index(env, "PWD");
			if (a >= 0)
				remove_env_entry(env, a);
			new_path = get_current_working_directory(100, 5, _out_fd[1]);
			if (new_path)
			{
				update_logical_pwd(env, new_path);
				update_pwd_env(new_path, env, a);
			}
			return (0);
		}
	}
	return (256);
}

static int	get_var_name_length(char *cmd, int equal_pos)
{
	if (equal_pos == -1)
		return (string_length(cmd, '\0'));
	else if (equal_pos > 0 && cmd[equal_pos - 1] == '+')
	{
		if (equal_pos > 1 && cmd[equal_pos - 2] == '+')
			return (-1);
		return (equal_pos - 1);
	}
	else
		return (equal_pos);
}

static int	get_return_value(int equal_pos, int var_name_len, char *cmd)
{
	if (equal_pos == -1)
		return (0);
	else if (equal_pos > 0 && cmd[equal_pos - 1] == '+')
		return (var_name_len);
	else
		return (equal_pos);
}

int	export_statment_check(char *_cmd_)
{
	int	len;
	int	equal_pos;
	int	var_name_len;

	if (!_cmd_ || !*_cmd_)
		return (-2);
	len = string_length(_cmd_, '\0');
	equal_pos = find_equal_pos(_cmd_, len);
	var_name_len = get_var_name_length(_cmd_, equal_pos);
	if (var_name_len == -1)
		return (-2);
	if (!validate_var_name(_cmd_, var_name_len))
		return (-2);
	return (get_return_value(equal_pos, var_name_len, _cmd_));
}
