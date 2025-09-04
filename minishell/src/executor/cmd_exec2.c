/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:34:02 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/17 05:43:08 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_or_reset_pipe_state(int *_piped, int f)
{
	int	a;

	a = 0;
	_piped[PIPE_ACTIVE] = _piped[PIPE_TOTAL];
	if (f)
	{
		while (a < PIPE_STATE_SIZE)
			_piped[a++] = 0;
	}
	else if (_piped[PIPE_TOTAL])
		_piped[PIPE_ACTIVE] += 1;
	_piped[PIPE_EXEC_STATE] = 1;
}

int	switch_fds_identifier(int *_piped, int index, int index_2, int con)
{
	if (con)
	{
		if (_piped[index] && _piped[index_2] != -1)
			safe_close(_piped[index_2]);
		_piped[index] = 1;
	}
	else
	{
		ft_putendl_fd("err: file not found", 2);
		_piped[PIPE_HAS_INPUT] = 0;
	}
	return (1);
}

int	check_if_command_is_builtin(char *_cmd)
{
	char	*tmp_cmd;
	int		status;

	status = 0;
	tmp_cmd = galloc(string_length(_cmd, ' ') + 1);
	if (!tmp_cmd)
		return (0);
	duplicate_string(tmp_cmd, _cmd, 0, string_length(_cmd, ' '));
	if (str_cmp(tmp_cmd, "echo", "cd") || str_cmp(tmp_cmd, "pwd", "export")
		|| str_cmp(tmp_cmd, "unset", "env") || str_cmp(tmp_cmd, "exit", NULL))
		status = 1;
	return (status);
}

int	run_builtin_in_child(char **_cmd_, t_env *env, int *_out_fd)
{
	int		status;
	int		last_idx;
	char	tmp_buf[4096];

	last_idx = 0;
	status = 0;
	while (_cmd_[last_idx])
		last_idx++;
	if (last_idx > 0)
	{
		ft_strlcpy(tmp_buf, "_=", sizeof(tmp_buf));
		ft_strlcat(tmp_buf, _cmd_[last_idx - 1], sizeof(tmp_buf));
		update_environment_var(tmp_buf, env);
	}
	if (str_cmp(_cmd_[0], "echo", NULL))
		status = echo_cmd(_cmd_, _out_fd);
	else if (str_cmp(_cmd_[0], "pwd", "env"))
		status = env_or_pwd_cmd(_cmd_[0], env, 0, _out_fd);
	else if (str_cmp(_cmd_[0], "export", "unset"))
		_cmd_ = unset_or_export_cmd(_cmd_, env, _out_fd, &status);
	else if (str_cmp(_cmd_[0], "cd", NULL))
		status = cd_cmd(_cmd_, env, _out_fd);
	else if (str_cmp(_cmd_[0], "exit", NULL))
		close_session(_cmd_);
	return (status);
}
