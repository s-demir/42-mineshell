/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:59:13 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/24 05:29:40 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	create_appended_var(char *var_name, char *old_val, char *new_part,
		t_env *env)
{
	char	*combined;
	int		old_len;
	int		new_len;
	int		var_len;

	var_len = string_length(var_name, '\0');
	old_len = string_length(old_val, '\0');
	new_len = string_length(new_part, '\0');
	combined = galloc(old_len + new_len + var_len + 2);
	duplicate_string(combined, var_name, 0, var_len);
	combined[var_len] = '=';
	duplicate_string(combined + var_len + 1, old_val, 0, old_len);
	duplicate_string(combined + var_len + 1 + old_len, new_part, 0, new_len);
	update_environment_var(combined, env);
}

static void	create_new_var(char *var_name, char *value, int var_len, t_env *env)
{
	char	*new_var;
	int		val_len;

	val_len = string_length(value, '\0');
	new_var = galloc(var_len + val_len + 2);
	duplicate_string(new_var, var_name, 0, var_len);
	new_var[var_len] = '=';
	duplicate_string(new_var + var_len + 1, value, 0, val_len);
	update_environment_var(new_var, env);
}

void	handle_append_export(char *cmd, t_env *env, int var_len)
{
	char	*var_name;
	int		existing_idx;
	char	*old_value;
	char	*new_value_part;

	var_name = galloc(var_len + 1);
	duplicate_string(var_name, cmd, 0, var_len);
	existing_idx = locate_env_var_index(env, var_name);
	new_value_part = cmd + var_len + 2;
	if (existing_idx >= 0)
	{
		old_value = env->parsed_env[existing_idx][1];
		create_appended_var(var_name, old_value, new_value_part, env);
	}
	else
		create_new_var(var_name, new_value_part, var_len, env);
}

int	echo_cmd(char **_cmd, int *_out_fd)
{
	int	a;
	int	op_n;

	op_n = 0;
	if (_cmd[0] && _cmd[1] && is_valid_echo_option(_cmd[1]))
		op_n = 1;
	a = op_n + 1;
	while (op_n && _cmd[a] && is_valid_echo_option(_cmd[a]))
		a++;
	if ((_cmd[0] && _cmd[a]) || string_length(_cmd[a], '\0'))
	{
		while (1)
		{
			ft_putstr_fd(_cmd[a], _out_fd[1]);
			a++;
			if (_cmd[a])
				write(_out_fd[1], " ", 1);
			else
				break ;
		}
	}
	if (!op_n)
		write(_out_fd[1], "\n", 1);
	return (0);
}

char	**unset_or_export_cmd(char **_cmd, t_env *env, int *_out_fd, int *s)
{
	int	a;
	int	c;

	a = 1;
	*s = 0;
	if (_cmd[a] && str_cmp(_cmd[0], "unset", NULL) && env->parsed_env[0])
	{
		while (_cmd[a])
		{
			c = locate_env_var_index(env, _cmd[a]);
			if (c >= 0)
				remove_env_entry(env, c);
			else
				*s = 256;
			a++;
		}
	}
	else if (str_cmp(_cmd[0], "export", NULL))
	{
		if (is_export_with_args(_cmd))
			_cmd = export_cmd(_cmd, env, _out_fd, &s);
		else
			env_or_pwd_cmd("env", env, 1, _out_fd);
	}
	return (_cmd);
}
